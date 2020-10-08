#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "renderer/color.hpp"

namespace cg
{
    namespace material
    {
        using material = std::tuple<cg::color,cg::color,cg::color,cg::color,float>;

        // A material of a fixed color not affected by light
        void solid_color(const cg::color&);
        
        void apply_material(const cg::color& diff, const cg::color& amb, const cg::color& spec, const cg::color& emi, const float s);
    }
    
}

#endif // MATERIAL_HPP