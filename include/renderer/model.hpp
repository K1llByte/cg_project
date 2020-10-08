#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include "common/vertex.hpp"
#include "renderer/color.hpp"
#include "renderer/vbo.hpp"
#include <filesystem>


namespace cg 
{
    class model
    {
        private:
            const cg::vnt_bo vtx_nor_tex_buffers;
            const uint tex_id;

            std::array<cg::vec4,8> bounding_box;

            cg::color diffuse;
            cg::color ambient;
            cg::color specular;
            cg::color emission;
            float shininess;


        public:
            model(const cg::vnt_vectors&);

            model(const cg::vnt_vectors&, const uint);

            model(const cg::vnt_vectors& , const uint, const cg::color& diff, const cg::color& amb, const cg::color& spec, const cg::color& emi, const float s);
            
            void draw() const;

            //void set_diffuse(const cg::color& diff);
            //void set_ambient(const cg::color& amb);
            //void set_specular(const cg::color& spec);
            //void set_emission(const cg::color& emi);
            //void set_shininess(const float s);
    };


    uint load_texture(const std::filesystem::path& file_name);

    void bind_texture(const uint tex_id);

    std::array<cg::vec4,8> make_bounding_box(const std::vector<cg::vec3>&);
}

#endif // MODEL_HPP