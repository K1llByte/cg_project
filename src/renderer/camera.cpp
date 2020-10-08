#include "renderer/camera.hpp"

#ifdef __APPLE__
#   include <GLUT/glut.h>
#   include <GLUT/glu.h>
#else
#   include <GL/glut.h>
#   include <GL/glu.h>
#endif

namespace cg
{
    camera::camera()
        : pos{0,0,0}, dir{0,0,0}, up{0,1,0} {}

    camera::camera(float t_x, float t_y, float t_z) 
        : pos{t_x,t_y,t_z}, dir{0,0,0}, up{0,1,0} {}

    camera::camera(const cg::vec3& t_pos, const cg::vec3& t_dir, const cg::vec3& t_up)
        : pos(t_pos), dir(t_dir), up(t_up) {}


    cg::vec3 camera::get_position() const
    {
        return pos;
    }


    cg::vec3 camera::get_direction() const
    {
        return dir;
    }


    cg::vec3 camera::get_up() const
    {
        return up;
    }


    void camera::set_position(const cg::vec3& t_pos)
    {
        this->pos = t_pos;
        //std::cout << "pos: " << this->pos_x << ' ' << this->pos_y << ' ' << this->pos_z << '\n'
        //    << "dir: " << this->dir_x << ' ' << this->dir_y << ' ' << this->dir_z << '\n' 
        //    << "u: " << this->ux << ' ' << this->uy << ' ' << this->uz << "\n\n";
    }

    void camera::set_direction(const cg::vec3& t_dir)
    {
        this->dir = t_dir;
    }

    void camera::set_up(const cg::vec3& t_up)
    {
        this->up = t_up;
    }

    void camera::place() const
    {
        //gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);
        gluLookAt(
            pos[0], pos[1], pos[2],
            dir[0], dir[1], dir[2],
             up[0],  up[1],  up[2]);
        
    }

    // DISGUST
    std::string camera::to_string() const
    {
        return "pos: (" + std::to_string(pos[0]) + "," + std::to_string(pos[1]) + "," + std::to_string(pos[2]) + ")\n" +
        "dir: (" + std::to_string(dir[0]) + "," + std::to_string(dir[1]) + "," + std::to_string(dir[2]) + ")\n" +
        "up: (" + std::to_string(up[0]) + "," + std::to_string(up[1]) + "," + std::to_string(up[2]) + ")";
    }
}