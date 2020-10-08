#ifndef LIGHT_HPP
#define LIGHT_HPP

#define MAX_LIGHTS 8

#include "common/vertex.hpp"
#include <GL/glew.h>
#ifdef __APPLE__
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif
#include <memory>

namespace cg
{
    class light;
    using lights = std::array<std::shared_ptr<cg::light>,MAX_LIGHTS>;

    const std::array<const GLenum,MAX_LIGHTS> GL_LIGHT{ GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, 
                                                        GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, 
                                                        GL_LIGHT6, GL_LIGHT7 };


    class light
    {
        protected:
            cg::vec4 diffuse;
            cg::vec4 ambient;
            cg::vec4 specular;

        public:
            light() : diffuse{1,1,1,1}, ambient{0.2,0.2,0.2,1}, specular{1,1,1,1} {};
            ~light() = default;

            virtual void apply(const GLenum) const = 0;

    };

    class directional_light: public light
    {
        private:
            cg::vec3 direction;
        public:
            directional_light(const cg::vec3& direction);

            void apply(const GLenum) const;
    };


    class point_light: public light
    {
        private:
            cg::vec3 position;
            float attenuation;
        public:
            point_light(const cg::vec3& position, const float attenuation = 0.0f);

            void apply(const GLenum) const;
    };


    class spot_light: public light
    {
        private:
            cg::vec3 position;
            cg::vec3 direction;
            float cutoff;
            float attenuation;
        public:
            spot_light(const cg::vec3& t_position, const cg::vec3& t_direction, const float cutoff, const float attenuation = 0.0f);

            void apply(const GLenum) const;
    };
}



#endif // LIGHT_HPP