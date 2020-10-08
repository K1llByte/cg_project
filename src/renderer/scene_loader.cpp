#include "renderer/scene_loader.hpp"

#include "renderer/model.hpp"
#include "generator/gen.hpp"
#include "generator/gen_parser.hpp"
#include "common/vertex.hpp"
#include "common/utils.hpp"
#include "renderer/light.hpp"

#include "lib/rapidxml_utils.hpp"

#include <iostream>
#include <string>
#include <chrono>

#include <unordered_map>

#define LOG(txt) std::cout << txt << '\n'

#define TEACHER_ON_THE_WATCH // If this is defined then the xml parser only allows 1 transformation type per group
#ifdef TEACHER_ON_THE_WATCH
    #define T_USED 1 // Transformation was used
    #define R_USED 2 // Rotation was used
    #define S_USED 4 // Scale was used
#endif


namespace cg
{
    
    template<int N>
    void attr_aux(rapidxml::xml_node<>* node, std::array<float,N>& args, const std::array<std::string,N>& attr_names)
    {
        for(auto attr_it = node->first_attribute() ; attr_it ; attr_it = attr_it->next_attribute())
        {
            for(uint i = 0 ; i < N/* attr_names.size() */ ; ++i)
            {
                if(attr_it->name() == attr_names[i])
                {
                    args[i] = std::stof(attr_it->value());
                }
            }
        }
    }

    using cg::vec3;

    std::vector<vec3> points_aux(rapidxml::xml_node<>* node, const float scaling)
    {
        std::vector<vec3> res(0);

        // Specialization of 'attr_aux' for scaling points
        for(auto node_it = node ; node_it ; node_it = node_it->next_sibling())
        {
            std::array<float,3> args = { 0 , 0 , 0 };
            std::array<std::string,3> attr_names = { "X" , "Y" , "Z" };

            for(auto attr_it = node_it->first_attribute() ; attr_it ; attr_it = attr_it->next_attribute())
            {
                for(uint i = 0 ; i < 3 ; ++i)
                {
                    if(attr_it->name() == attr_names[i])
                    {
                        args[i] = scaling * std::stof(attr_it->value());
                    }
                }
            }
            res.push_back(args);
        }

        return res;
    }


    std::optional<cg::color> xml_attrib_to_color(rapidxml::xml_attribute<>* color_attr, const std::unordered_map<std::string,cg::color>& color_map)
    {
        //auto color_attr = node->first_attribute("color");

        cg::color clr;
        if(color_attr != nullptr)
        {
            const auto& clr_name = std::string(color_attr->value());
            if(clr_name == "random")
            {
                //LOG("> found random");
                clr = cg::color(
                    float(rand()) / float(RAND_MAX),
                    float(rand()) / float(RAND_MAX),
                    float(rand()) / float(RAND_MAX));
            }
            else if(clr_name == "rgb")
            {
                //LOG("> found rgb");
                clr = cg::color(-1,-1,-1);
            }
            else
            {
                try
                {
                    clr = color_map.at(clr_name);
                }
                catch(const std::out_of_range& e)
                {
                    std::cerr << "> Color not found\n";
                    return std::nullopt;
                }
            }  
        }
        else
        {
            return std::nullopt;
            //clr = cg::color(1.f,1.f,1.f,1.f);
        }

        return clr;
    }



    std::optional<cg::model> xml_node_to_model(rapidxml::xml_node<>* node, const std::unordered_map<std::string,cg::color>& color_map)
    {
        bool is_cmd = false;

        const auto command_attr = node->first_attribute("command");
        if(command_attr != nullptr)
        {
            is_cmd = true;
        }

        //LOG("> MODEL");
        const auto file_attr = node->first_attribute("file");
        // If attribute file doesn't exists then there is no model
        if(file_attr == nullptr && !is_cmd)
        {
            std::cerr << "> Model parse fail\n";
            return std::nullopt;
        }

        // ===== LOAD DIFFUSE COLOR ================================================ //

        const auto& opt_diff = xml_attrib_to_color(node->first_attribute("diffuse"), color_map);
        cg::color diff(0.8,0.8,0.8,1);
        if(opt_diff != std::nullopt)
        {
            diff = opt_diff.value();
        }
        

        // ===== LOAD AMBIENT COLOR ===== //

        const auto& opt_amb = xml_attrib_to_color(node->first_attribute("ambient"), color_map);
        cg::color amb{0.2,0.2,0.2,1};
        if(opt_amb != std::nullopt)
        {
            amb = opt_amb.value();
        }

        // ===== LOAD SPECULAR COLOR ===== //

        const auto& opt_spec = xml_attrib_to_color(node->first_attribute("specular"), color_map);
        cg::color spec{0,0,0,1};
        if(opt_spec != std::nullopt)
        {
            spec = opt_spec.value();
        }

        

        // ===== LOAD EMISSION COLOR ===== //

        const auto& opt_emi = xml_attrib_to_color(node->first_attribute("emission"), color_map);
        cg::color emi{0,0,0,1};
        if(opt_emi != std::nullopt)
        {
            emi = opt_emi.value();
        }

        const auto& shininess_attrib = node->first_attribute("shininess");
        float shiny = 0;
        if(shininess_attrib != nullptr)
        {
            try
            {
                shiny = std::stof(shininess_attrib->value());   
            }
            catch(const std::exception& e)
            {
                std::cerr << "> Shininess stof fail\n";
                return std::nullopt;
            }
        }

        // ==================================================================== //

        
        auto texture_attr = node->first_attribute("texture");
        uint tex_id = 0;
        if(texture_attr != nullptr)
        {
            tex_id = cg::load_texture(texture_attr->value());
        }

        if(is_cmd)
        {
            try
            {
                return cg::model(gen_by_command(command_attr->value()) , tex_id , diff, amb, spec, emi, shiny);
            }
            catch(const std::exception& e)
            {   
                std::cerr << "> Invalid primitive generation command\n";
                return std::nullopt;
            }
        }
        else
        {
            auto vtxs = cg::file_to_vertices(std::string(file_attr->value()));
            if(!vtxs.has_value())
            {
                std::cerr << "> Invalid 3d model file\n";
                return std::nullopt;
            }
            return  cg::model(vtxs.value() , tex_id, diff, amb, spec, emi, shiny);
        }
    }



    std::optional<group> xml_node_to_group(rapidxml::xml_node<>* parent_node, const std::unordered_map<std::string,cg::color>& color_map)
    {
        #ifdef TEACHER_ON_THE_WATCH
            char used_transformations = 0;
        #endif

        cg::group grp = cg::group();
        // Nodes inside first group
        for(auto node_it = parent_node->first_node() ; node_it != nullptr ; node_it = node_it->next_sibling())
        {
            // =========== Transformation Parsing =========== //
            // =========== Translation Parsing =========== //
            if(std::string(node_it->name()) == "translate")
            {
                #ifdef TEACHER_ON_THE_WATCH
                    if(used_transformations & T_USED)
                    {
                        
                        std::cerr << "> Translate already used in this group\n";
                        return std::nullopt;
                    }
                #endif


                //LOG("> TRANSLATE");
                try
                {
                    auto fst_attr = node_it->first_attribute("time");
                    float time;
                    if(fst_attr != nullptr)
                    {
                        float scaling = 1.f;
                        bool closed = true;
                        
                        for(auto attr_it = node_it->first_attribute() ; attr_it ; attr_it = attr_it->next_attribute())
                        {
                            if(std::string(attr_it->name()) == "scaling")
                            {
                                scaling = std::stof(attr_it->value());
                                if(scaling < 0)
                                {
                                    return std::nullopt;
                                }
                            }
                            else if(std::string(attr_it->name()) == "closed")
                            {
                                if(std::string(attr_it->value()) == "true")
                                    closed = true;
                                else if(std::string(attr_it->value()) == "false")
                                    closed = false;
                                else
                                    return std::nullopt;
                            }
                        }

                        time = std::stof(fst_attr->value());
                        std::vector<vec3> points = points_aux(node_it->first_node(),scaling);

                        if(points.size() < 4)
                        {
                            std::cerr << "> Insuficient points for trajectory\n";
                            return std::nullopt;
                        }

                        grp.add_transformation(cg::translation_timed(time,points,closed));
                    }
                    else
                    {
                        std::array<float,3> args{ 0 , 0 , 0 };
                        attr_aux<3>(node_it,args,{ "X" , "Y" , "Z" });

                        grp.add_transformation(cg::translation(args[0],args[1],args[2]));
                    }                    
                }
                catch(const std::exception& e)
                {
                    LOG("> Translation stof fail");
                    return std::nullopt;
                }

                #ifdef TEACHER_ON_THE_WATCH
                    used_transformations |= T_USED;
                #endif
            }
            // =========== Rotation Parsing =========== //
            else if(std::string(node_it->name()) == "rotate")
            {
                #ifdef TEACHER_ON_THE_WATCH
                    if(used_transformations & R_USED)
                    {
                        std::cerr << "> Rotate already used in this group\n";
                        return std::nullopt;
                    }
                #endif

                //LOG("> ROTATE");
                try
                {

                    std::array<float,4> args{ 0 , 0 , 0 , 0 };
                    if(node_it->first_attribute("time") != nullptr)
                    {
                        attr_aux<4>(node_it,args,{ "time" , "axisX" , "axisY" , "axisZ" });
                        grp.add_transformation(cg::rotation_timed(args[0], args[1], args[2], args[3]));
                    }
                    else
                    {
                        attr_aux<4>(node_it,args,{ "angle" , "axisX" , "axisY" , "axisZ" });
                        grp.add_transformation(cg::rotation(args[0], args[1], args[2], args[3]));
                    }
                }
                catch(const std::exception& e)
                {
                    //LOG("> Rotation stof fail");
                    return std::nullopt;
                }

                #ifdef TEACHER_ON_THE_WATCH
                    used_transformations |= R_USED;
                #endif
            }
            // =========== Scale Parsing =========== //
            else if(std::string(node_it->name()) == "scale")
            {
                #ifdef TEACHER_ON_THE_WATCH
                    if(used_transformations & S_USED)
                    {
                        std::cerr << "> Scale already used in this group\n";
                        return std::nullopt;
                    }
                #endif

                //LOG("> SCALE");
                try
                {
                    std::array<float,3> args{ 1 , 1 , 1 };
                    attr_aux<3>(node_it,args,{ "X" , "Y" , "Z" });
                    grp.add_transformation(cg::scale(args[0],args[1],args[2]));
                }
                catch(const std::exception& e)
                {
                    //LOG("> Scale stof fail");
                    return std::nullopt;
                }

                #ifdef TEACHER_ON_THE_WATCH
                    used_transformations |= S_USED;
                #endif
            }
            
            // =========== Models Parsing =========== //
            else if(std::string(node_it->name()) == "models" && node_it->first_attribute() == nullptr)
            {
                //LOG("> MODELS");
                for(auto models_it = node_it->first_node(); models_it ; models_it = models_it->next_sibling())
                {
                    if(std::string(models_it->name()) != "model")
                    {
                        std::cerr << "> Invalid node in models\n";
                        return std::nullopt;
                    }
                    const auto model = xml_node_to_model(models_it,color_map);
                    if(model == std::nullopt)
                    {
                        std::cerr << "> Model in <models> parse fail\n";
                        return std::nullopt;
                    }
                    grp.add_model(model.value());
                }
                
            }
            // =========== Model Parsing =========== //
            else if(std::string(node_it->name()) == "model")
            {
                const auto model = xml_node_to_model(node_it,color_map);
                if(model == std::nullopt)
                {
                    std::cerr << "> Model parse fail\n";
                    return std::nullopt;
                }
                grp.add_model(model.value());
                
            }
            // =========== Group Parsing =========== //
            else if(std::string(node_it->name()) == "group" && node_it->first_attribute() == nullptr)
            {
                //LOG("> GROUP");
                auto tmp = xml_node_to_group(node_it,color_map);
                if(tmp == std::nullopt)
                {
                    std::cerr << "> Group parse fail\n";
                    return std::nullopt;
                }
                grp.add_group(tmp.value());
            }
            else
            {
                return std::nullopt;
            }
            
        }

        return std::make_optional<>(grp);
    }



    std::optional<cg::lights> xml_node_to_lights(rapidxml::xml_node<>* parent_node)
    {
        cg::lights res;
        uint8_t counter = 0;

        for(auto node_it = parent_node->first_node() ; node_it != nullptr ; node_it = node_it->next_sibling())
        {
            if(std::string(node_it->name()) == "light")
            {
                if(counter >= MAX_LIGHTS)
                {
                    std::cerr << "> Not allowed more than 8 lights!\n";
                    return std::nullopt; 
                }

                const auto type_attr = node_it->first_attribute("type");
                if(type_attr == nullptr)
                {
                    std::cerr << "> 'type' attribute not found!\n";
                    return std::nullopt; 
                }

                // According type value argument parsing will go according to light type
                std::string type_value(type_attr->value());
                if(type_value == "POINT")
                {
                    std::array<float,4> args{ 0 , 0 , 0 , 0 };
                    attr_aux<4>(node_it,args,{ "posX", "posY", "posZ", "attenuation" });
                    res[counter++] = std::make_shared<cg::point_light>(cg::vec3{args[0],args[1],args[2]}, args[3]);
                }
                else if(type_value == "DIRECTIONAL")
                {
                    std::array<float,3> args{ 1 , 1 , 0 };
                    attr_aux<3>(node_it,args,{ "dirX", "dirY", "dirZ" });
                    res[counter++] = std::make_shared<cg::directional_light>( cg::vec3{args[0],args[1],args[2]} );
                }
                else if(type_value == "SPOT")
                {
                    std::array<float,8> args{ 0 , 0 , 0 , 0 , 1 , 1 , 45.f , 0};
                    std::array<std::string,8> tmp{ "posX", "posY", "posZ", "dirX", "dirY", "dirZ", "cutoff", "attenuation" };
                    attr_aux<8>(node_it,args,tmp);
                    res[counter++] = std::make_shared<cg::spot_light>(cg::vec3{args[0],args[1],args[2]}, cg::vec3{args[3],args[4],args[5]}, args[6], args[7] );
                }
                else
                {
                    std::cerr << "> 'type' attribute value not recognized!\n";
                    return std::nullopt; 
                }
                
                //TODO: Create shared pointer with light
                //TODO: Increment counter
            }
            else
            {
                std::cerr << "> Other type of nodes are not allowed here!\n";
                return std::nullopt;
            }
        }
        return std::make_optional<>(res);
    }



    std::optional<scene> xml_to_scene(const std::filesystem::path& file_name)
    {
        if(!std::filesystem::exists(file_name))
        {
            std::cerr << "> File doesn't exists\n";
            return std::nullopt;
        }

        rapidxml::xml_document<> doc;
        rapidxml::file<> xml_file(file_name.c_str());
        
        try
        { 
            doc.parse<0>(xml_file.data()); 
        }
        catch(const rapidxml::parse_error&)
        { 
            std::cerr << "> XML Parse Error\n";
            return std::nullopt; 
        }

        // =========== Colors Parsing =========== //
        std::unordered_map<std::string,cg::color> color_map;
        auto colors_node = doc.first_node("colors");
        if(colors_node != nullptr)
        {
            if(colors_node->next_sibling("colors") != nullptr)
            {
                std::cerr << "> No scene\n";
                return std::nullopt;
            }

            for(auto node_it = colors_node->first_node(); node_it ; node_it = node_it->next_sibling())
            {
                if(std::string(node_it->name()) == "color")
                {
                    auto name = node_it->first_attribute("name");
                    if(name == nullptr)
                    {   
                        std::cerr << "> Color name required\n";
                        return std::nullopt;
                    }

                    std::array<float,4> args{ 0.f, 0.f, 0.f, 1.f };
                    //const std::array<std::string,4> attr_names{ "r", "g", "b", "a" };
                    try
                    {
                        attr_aux<4>(node_it,args,{"r","g","b","a"});
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << "> Color stof fail\n";
                        return std::nullopt;
                    }

                    // Input validation
                    for(uint i = 0 ; i < 4 ; ++i)
                        if(args[i] < 0 || args[i] > 1)
                        {
                            std::cerr << "> Invalid color values range [0,1]\n";
                            return std::nullopt;
                        }
                    color_map[name->value()] = cg::color(args);
                }
                else
                {
                    std::cerr << "> <colors> No matching node\n";
                    return std::nullopt;
                }
            }
        }

        // =========== Scene Parsing =========== //
        auto scene_node = doc.first_node("scene");
        if(scene_node == nullptr || scene_node->next_sibling("scene") != nullptr)
        {
            std::cerr << "> No scene\n";
            return std::nullopt;
        }
        

        std::vector<cg::group> grps;
        cg::camera c = cg::camera();
        cg::model* skybox = nullptr;
        cg::lights lts;
        
        srand(time(nullptr));
        for(auto node_it = scene_node->first_node(); node_it ; node_it = node_it->next_sibling())
        {
            if(std::string(node_it->name()) == "group")
            {
                const auto tmp = xml_node_to_group(node_it,color_map);
                if(tmp == std::nullopt)
                {
                    std::cerr << "> Group parse fail\n";
                    return std::nullopt;
                }
                grps.push_back(tmp.value());
            }
            else if(std::string(node_it->name()) == "camera")
            {
                std::array<float,3> args{ 0 , 0 , 0 };
                //const std::array<std::string,3> attr_names{ "posX" , "posY" , "posZ" };
                try
                {
                    attr_aux<3>(node_it,args,{ "posX" , "posY" , "posZ" });
                }
                catch(const std::exception& e)
                {
                    std::cerr << "> Camera stof fail\n";
                    return std::nullopt;
                }
                
                
                c.set_position(args);
            }
            else if(std::string(node_it->name()) == "lights")
            {
                const auto& tmp = xml_node_to_lights(node_it);
                if(tmp == std::nullopt)
                {
                    std::cerr << "> Lights parse fail\n";
                    return std::nullopt;
                }
                lts = tmp.value();
            }
            else if(std::string(node_it->name()) == "skybox")
            {
                

                
                const auto width_attr = node_it->first_attribute("width");
                float width = 500.0f;

                if(width_attr != nullptr)
                {
                    width = std::stof(width_attr->value());
                }

                const auto texture_attr = node_it->first_attribute("texture");

                if(texture_attr == nullptr)
                {
                    std::cerr << "> Texture file required\n";
                    return std::nullopt;
                }

                uint tex_id = cg::load_texture(texture_attr->value());
                
                if(tex_id == 0)
                {
                    std::cerr << "> Texture file not found\n";
                    return std::nullopt;
                }

                const cg::color null_clr{0,0,0,1};
                skybox = new cg::model(gen_skybox(width),tex_id,null_clr,null_clr,null_clr,cg::color{1,1,1,1},128);
            }
            else
            {
                std::cerr << "> Groups parse fail\n";
                return std::nullopt;
            }
        }
        
        if(skybox == nullptr)
        {
            cg::scene scn(grps,lts,c);
            return std::make_optional<>(scn);
        }
        else
        {
            cg::scene scn(grps,*skybox,lts,c);
            return std::make_optional<>(scn);
        }
    }
}