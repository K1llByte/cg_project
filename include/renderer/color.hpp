#ifndef COLOR_HPP
#define COLOR_HPP

#include "common/vertex.hpp"

#include <array>

namespace cg
{
    class color
    {
        private:
            cg::vec4 rgba;
            //bool rgb;

        public:

            color();
            color(float t_r, float t_g, float t_b);
            color(float t_r, float t_g, float t_b, float t_a);
            color(const cg::vec3& t_v);
            color(const cg::vec4& t_v);
            //color(const bool& rgb);
            color(const color& t_clr) = default;
            //~color() = default;

            constexpr float get_r() const { return rgba[0]; }
            constexpr float get_g() const { return rgba[1]; }
            constexpr float get_b() const { return rgba[2]; }
            constexpr float get_a() const { return rgba[3]; }
            

            float operator[](char channel) const;
            float operator[](int pos) const;

            constexpr cg::vec4 data() const { return rgba; }

            static constexpr cg::vec4 normalize_color(const cg::vec4& nn)
            {
                cg::vec4 tmp{ nn[0]/255.f, nn[1]/255.f, nn[2]/255.f, nn[3]/255.f };
                return tmp;
            }
    };


    

    const cg::vec3 rgb(const float speed = 100.f);
}



#endif // COLOR_HPP