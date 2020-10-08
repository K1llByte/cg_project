#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "common/vertex.hpp"

#include <iostream>
#include <array>
//#include <bitset>

namespace cg
{
    class camera
    {
        private:

            cg::vec3 pos;
            cg::vec3 dir;
            cg::vec3 up;
            //std::bitset<2> bit_opt;
        
        public:
            camera();
            camera(const float t_x, const float t_y, const float t_z);
            camera(const cg::vec3& t_pos, const cg::vec3& t_dir, const cg::vec3& t_up);

            cg::vec3 get_position() const;
            cg::vec3 get_direction() const;
            cg::vec3 get_up() const;

            void set_position(const cg::vec3&);
            void set_direction(const cg::vec3&);
            void set_up(const cg::vec3&);

            void place() const;

            std::string to_string() const; // Disgust
    };
}

#endif // CAMERA_HPP
