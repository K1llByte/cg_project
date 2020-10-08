#ifndef VBO_HPP
#define VBO_HPP

#include <GL/glew.h>
#ifdef __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/glut.h>
#endif
#include <iostream>
#include <vector>

#include "common/vertex.hpp"
#include "renderer/color.hpp"

namespace cg
{
    // Vertex Buffer Object
    class vbo
    {
        

        private:
            uint vbo_id;
            uint num_vertices;

        public:

            vbo() = default; // TODO: delete this ctor
            vbo(const std::vector<vec3>& vertices);

            void draw(const GLenum mode = GL_TRIANGLES, const uint first = 0, const int count = -1, const cg::color& clr = cg::color()) const;
    };


    // Vertex, Normal and Texture Buffer Object
    class vnt_bo
    {
        private:
            uint vertices_id;
            uint normals_id;
            uint textures_id;
            uint num_vnt;

        public:

            vnt_bo() = default; // TODO: delete this ctor
            vnt_bo(const cg::vnt_vectors& vnt_s);

            void draw(const GLenum mode = GL_TRIANGLES, const uint first = 0, const int count = -1) const;
    };
}

#endif // VBO_HPP