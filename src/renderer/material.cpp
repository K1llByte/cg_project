#include "renderer/material.hpp"

#ifdef __APPLE__
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

namespace cg
{
    namespace material
    {
        void solid_color(const cg::color& clr)
        {
            const float clr_null[4] = { 0.0 , 0.0 , 0.0 , 1.0 };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, clr_null);
	        glMaterialfv(GL_FRONT, GL_AMBIENT, clr_null);
	        glMaterialfv(GL_FRONT, GL_SPECULAR, clr_null);
	        glMaterialfv(GL_FRONT, GL_EMISSION, clr.data().data());
	        glMaterialf(GL_FRONT, GL_SHININESS, 128);
        }


        void apply_material(const cg::color& diff, const cg::color& amb, const cg::color& spec, const cg::color& emi, const float s)
        {
            auto& tmp = rgb();
            const float tmp_rgb[4] = { tmp[0], tmp[1], tmp[2], 1.0f };

            glMaterialfv(GL_FRONT,  GL_DIFFUSE, (diff[0] < 0) ? tmp_rgb : diff.data().data());
	        glMaterialfv(GL_FRONT,  GL_AMBIENT,  (amb[0] < 0) ? tmp_rgb : amb.data().data());
	        glMaterialfv(GL_FRONT, GL_SPECULAR, (spec[0] < 0) ? tmp_rgb : spec.data().data());
	        glMaterialfv(GL_FRONT, GL_EMISSION,  (emi[0] < 0) ? tmp_rgb : emi.data().data());
	        glMaterialf(GL_FRONT, GL_SHININESS, s);
        }
    }
}