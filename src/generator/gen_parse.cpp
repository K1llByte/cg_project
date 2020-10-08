#include "generator/gen_parser.hpp"

#include "generator/gen.hpp"
#include "common/utils.hpp"


#include <string>
#include <iostream>
#include <stdexcept>


cg::vnt_vectors gen_by_strviews(const std::vector<std::string_view>& splited)
{
	if(splited.size() == 0)
        throw std::logic_error("Invalid number of arguments");

	if(splited[0] == "cylinder")
    {
        if(splited.size() < 4)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "cylinder [radius] [height] [slices]\n";
            throw std::invalid_argument("cylinder [radius] [height] [slices]");
        }

        float radius = std::stof(std::string(splited[1]));
        float height = std::stof(std::string(splited[2]));
        int slices = std::stoi(std::string(splited[3]));
 
        if(slices < 0 || height <= 0)
        {
            //std::cerr << "Error: Invalid Argument Range\n";
            throw std::out_of_range("slices or height");
        }
        
        return gen_cylinder(radius, height, slices);
    }
	else if(splited[0] == "sphere")
    {
        if(splited.size() < 4)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "sphere [radius] [slices] [stacks]\n";
            throw std::invalid_argument("sphere [radius] [slices] [stacks]");
        }

        float radius = std::stof(std::string(splited[1]));
        int slices = std::stoi(std::string(splited[2]));
        int stacks = std::stoi(std::string(splited[3]));

        // Unsigned conversion checking
        if(slices < 0 || stacks < 0)
        {
            throw std::out_of_range("slices or stacks");
        }
        
        return gen_sphere(radius, slices, stacks);
    }
	else if(splited[0] == "plane")
    {
        if(splited.size() < 2)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "plane [width]\n";
            throw std::invalid_argument("plane [width]");
        }

        float width = std::stof(std::string(splited[1]));

        return gen_plane(width);
    }
	else if(splited[0] == "box")
    {
        if(splited.size() < 5)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "box [width_x] [width_y] [width_z] [divisions]\n";
            throw std::invalid_argument("box [width_x] [width_y] [width_z] [divisions]");
        }

        float width_x = std::stof(std::string(splited[1]));
        float width_y = std::stof(std::string(splited[2]));
        float width_z = std::stof(std::string(splited[3]));
        int divisions = std::stoi(std::string(splited[4]));

        // Unsigned conversion checking
        if(divisions < 0)
        {
            std::cerr << "Error: Invalid Argument Range\n";
            exit(1);
        }
        
        return gen_box(width_x,width_y,width_z,divisions);
    }
	else if(splited[0] == "cone")
    {
        if(splited.size() < 5)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "cone [radius] [height] [slices] [stacks]\n";
            throw std::invalid_argument("cone [radius] [height] [slices] [stacks]");
        }

        float radius = std::stof(std::string(splited[1]));
        float height = std::stof(std::string(splited[2]));
        int slices = std::stoi(std::string(splited[3]));
        int stacks = std::stoi(std::string(splited[4]));

        // Unsigned conversion checking
        if(slices < 1 || stacks < 0)
        {
            throw std::out_of_range("slices or stacks");
        }
        
        return gen_cone(radius,height,slices,stacks);
    }
	else if(splited[0] == "ring")
    {
        if(splited.size() < 4)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "ring [inner_radius] [outter_radius] [slices]\n";
            throw std::invalid_argument("ring [inner_radius] [outter_radius] [slices]");
        }

        float inner_radius = std::stof(std::string(splited[1])); 
        float outter_radius = std::stof(std::string(splited[2]));
        int slices = std::stoi(std::string(splited[3]));

        // Unsigned conversion checking
        if(slices < 1)
        {
            throw std::out_of_range("slices");
        }
        
        return gen_ring(inner_radius,outter_radius,slices);
    }
	else if(splited[0] == "torus")
    {
        if(splited.size() < 5)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "torus [radius] [cyl_radius] [slices] [cyl_slices]\n";
            throw std::invalid_argument("torus [radius] [cyl_radius] [slices] [cyl_slices]");
        }

        float radius = std::stof(std::string(splited[1]));
        float cyl_radius = std::stof(std::string(splited[2]));
        int slices = std::stoi(std::string(splited[3]));
        int cyl_slices = std::stoi(std::string(splited[4]));

        // Unsigned conversion checking
        if(slices < 1 || cyl_slices < 1)
        {
            throw std::out_of_range("slices or cyl_slices");
        }
        
        return gen_torus(radius,cyl_radius,slices,cyl_slices);
    }
    else if(splited[0] == "patch")
    {
        if(splited.size() < 3)
        {
            //std::cerr << "Error: Invalid number of arguments\n";
            //std::cerr << "torus [radius] [cyl_radius] [slices] [cyl_slices]\n";
            throw std::invalid_argument("patch [patch_file] [num_divisions]");
        }

        int num_divisions = std::stoi(std::string(splited[2]));

        if(num_divisions < 1)
        {
            throw std::out_of_range("num_divisions");
        }

        std::optional<cg::patches> opt_patches = cg::file_to_patches(splited[1]);
        if(opt_patches == std::nullopt)
            throw std::logic_error("Invalid patches file");
        

        cg::patches patches = opt_patches.value();

        return gen_patches(std::get<0>(patches),std::get<1>(patches),num_divisions);
    }
	else
    {
		throw std::logic_error("No such primitive");
    }
}


cg::vnt_vectors gen_by_command(const std::string& cmd)
{
    return gen_by_strviews(str_utils::split(cmd));
}