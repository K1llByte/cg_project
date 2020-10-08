#include "common/vertex.hpp"

#include <fstream>
#include <iostream>
#include "common/utils.hpp"



namespace cg
{
    // vertex::vertex(float t_x, float t_y, float t_z) :
    //     v({t_x , t_y , t_z}) {}

    // vertex::vertex(const std::array<float,3>& t_v) :
    //     v(t_v) {}

    // std::string vertex::to_string() const
    // {
    //     return '(' + 
    //         std::to_string(v[0]) + ',' +
    //         std::to_string(v[1]) + ',' +
    //         std::to_string(v[2]) + ')';
    // }


    std::optional<cg::vnt_vectors> file_to_vertices(const std::filesystem::path& file_name)
    {
        std::ifstream fis;
        float in_x, in_y, in_z;
        uint in_size;
        fis.open(file_name);
        
        if(fis.good())
        {
            fis >> in_size; 
            if(in_size % 3 > 0)
                return std::nullopt;
        }
        else
            return std::nullopt;
        

        std::vector<cg::vec3> vtxs(in_size);
        

        for(uint i = 0 ; fis.good() && i < in_size ; ++i)
        {
            fis >> in_x >> in_y >> in_z;
            vtxs[i] = cg::vec3{in_x,in_y,in_z};
        }

        std::vector<cg::vec3> normals(in_size);

        for(uint i = 0 ; fis.good() && i < in_size ; ++i)
        {
            fis >> in_x >> in_y >> in_z;
            normals[i] = cg::vec3{in_x,in_y,in_z};
        }

        std::vector<cg::vec2> tex_coord(in_size);

        for(uint i = 0 ; fis.good() && i < in_size ; ++i)
        {
            fis >> in_x >> in_y;
            tex_coord[i] = cg::vec2{in_x,in_y};
        }

        fis.close();

        return std::make_tuple(vtxs,normals,tex_coord);
    }

    void vertices_to_file(const cg::vnt_vectors& vnt_mesh, const std::filesystem::path& file_name)
    {
        //TODO: IO exceptions
        std::ofstream fos;
        std::filesystem::create_directories(file_name.parent_path());
        fos.open(file_name);

        fos << std::get<0>(vnt_mesh).size() << '\n';
        for(const cg::vec3& vtx : std::get<0>(vnt_mesh))
        {
            fos << vtx[0] << ' ' <<
                   vtx[1] << ' ' <<
                   vtx[2] << '\n';
        }

        for(const cg::vec3& nor : std::get<1>(vnt_mesh))
        {
            fos << nor[0] << ' ' <<
                   nor[1] << ' ' <<
                   nor[2] << '\n';
        }

        for(const cg::vec2& tex : std::get<2>(vnt_mesh))
        {
            fos << tex[0] << ' ' <<
                   tex[1] << '\n';
        }

        fos.close();
    }

    std::optional<patches> file_to_patches(const std::filesystem::path& file_name)
    {
        std::ifstream fis;
        fis.open(file_name);
        std::string line;
        
        // Get number of patches
        uint num_patches;
        if(fis.good())
        {
            std::getline(fis,line);
            num_patches = std::stoi(line);
        }
        else
            return std::nullopt;
        

        // Get all patches indices
        std::vector<std::array<float,16>> patches_indices(num_patches);
        for(uint i = 0 ; i < num_patches ; ++i)
        {
            if(fis.good())
            {
                std::getline(fis,line);
                std::stringstream ss(line);
                std::string in_str_idx;
                for(uint j = 0 ; j < 16 && ss.good() ; ++j)
                {
                    std::getline(ss,in_str_idx,',');
                    patches_indices[i][j] = std::stoi(in_str_idx);
                }
            }
        }

        // Get number of control points
        uint num_control_points;
        if(fis.good())
        {
            std::getline(fis,line);
            num_control_points = std::stoi(line);
        }
        else
            return std::nullopt;

        // Get control points
        std::vector<cg::vec3> control_points(num_control_points);


        for(uint i = 0 ; fis.good() && i < num_control_points ; ++i)
        {
            std::getline(fis,line);
            std::stringstream ss(line);
            std::string in_str_coord;

            vec3 in_vec;
            for(uint i_coord = 0 ; i_coord < 3 && ss.good() ; ++i_coord)
            {
                std::getline(ss,in_str_coord,',');
                in_vec[i_coord] = std::stof(in_str_coord);
            }
            control_points[i] = in_vec;
        }

        return std::make_tuple(patches_indices,control_points);
    }
}