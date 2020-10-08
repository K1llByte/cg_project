#include "renderer/light.hpp"

#include <GL/glew.h>
#ifdef __APPLE__
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

namespace cg
{
    directional_light::directional_light(const cg::vec3& t_direction)
        : direction(t_direction) {}

    void directional_light::apply(const GLenum light_num) const
    {
        const float dir[4] = { direction[0], direction[1], direction[2], 0.0 };
        glLightfv(light_num, GL_AMBIENT, ambient.data());
        glLightfv(light_num, GL_DIFFUSE, diffuse.data());
        glLightfv(light_num, GL_SPECULAR, specular.data());
        glLightfv(light_num, GL_POSITION, dir);
    }



    point_light::point_light(const cg::vec3& t_position, const float t_attenuation)
        : position(t_position), attenuation(t_attenuation) {}
        
    void point_light::apply(const GLenum light_num) const
    {
        const float pos[4] = { position[0], position[1], position[2], 1.0 };
        glLightfv(light_num, GL_AMBIENT, ambient.data());
        glLightfv(light_num, GL_DIFFUSE, diffuse.data());
        glLightfv(light_num, GL_SPECULAR, specular.data());

        glLightfv(light_num, GL_POSITION, pos);
        glLightf(light_num, GL_QUADRATIC_ATTENUATION, attenuation);
    }



    spot_light::spot_light(const cg::vec3& t_position, const cg::vec3& t_direction, const float t_cutoff, const float t_attenuation)
        : position(t_position), direction(t_direction), cutoff(t_cutoff), attenuation(t_attenuation) {}

    void spot_light::apply(const GLenum light_num) const
    {
        const float pos[4] = { position[0], position[1], position[2], 1.0 };
        glLightfv(light_num, GL_AMBIENT, ambient.data());
        glLightfv(light_num, GL_DIFFUSE, diffuse.data());
        glLightfv(light_num, GL_SPECULAR, specular.data());

        glLightfv(light_num, GL_POSITION, pos);
        glLightfv(light_num, GL_SPOT_DIRECTION, direction.data());
        glLightf(light_num, GL_SPOT_CUTOFF, cutoff);
        glLightf(light_num, GL_QUADRATIC_ATTENUATION, attenuation);
    }
}