#include "renderer/transformation.hpp"

#include <GL/glew.h>
#ifdef __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/glut.h>
#endif

#include "renderer/scene.hpp"
#include "renderer/material.hpp"

#include <cmath>
#include <stdexcept>
#include <iostream>


namespace cg
{
    //extern bool scene::debug;
    bool scene::debug;

    // ============ Auxiliary function for catmul rom spline calculation ============ //

    const vec3 catmul_rom(const std::vector<vec3>& points, const float time , const bool closed = true)
    {

        // 'time' is the time relative to the positional time of the trajectory
        // 'nt' is the time between 0-1 used in catmul_rom
        // 'selection_time' is the value calculated from 'time' that is used to select the 4 positions for catmul_rom

        const uint size = points.size();
        
        uint i0;
        uint i1;
        uint i2;
        uint i3;

        if(!closed)
		{
			uint idx = ((int) floor(time)) % (size-1);
			i0 = (idx==0) ? 0 : idx-1;
			i1 = idx;
			i2 = idx+1;
			i3 = idx + ((idx+2 == size) ? 1 : 2);	
		}
		else 
		{
			uint idx = floor(time);
			i0 = (idx+size-1)%size;
			i1 = (i0+1)%size;
			i2 = (i1+1)%size;
			i3 = (i2+1)%size;
		}

        const float nt = time - std::floor(time);

        const vec4 ts = {
            -0.5f*(nt*nt*nt) +(nt*nt)      -0.5f*nt ,
             1.5f*(nt*nt*nt) -2.5f*(nt*nt) +1.0f                 ,
            -1.5f*(nt*nt*nt) +2.0f*(nt*nt) +0.5f*nt ,
             0.5f*(nt*nt*nt) -0.5f*(nt*nt)                      };

        const std::array<vec3,4> mat{ points[i0] , points[i1] , points[i2] , points[i3] };

        // Dot product
        vec3* res = new vec3();
        for(size_t j = 0 ; j < 3 ; ++j)
            for(size_t k = 0 ; k < 4 ; ++k)
                (*res)[j] += ts[k] * mat[k][j];

        return *res;
    }

    // ======================================================================= //

    translation::translation(float t_x, float t_y, float t_z) 
        : x(t_x), y(t_y), z(t_z) {}
    
    void translation::apply(const float) 
    {
        glTranslatef(x,y,z);
    }



    rotation::rotation(float t_angle, float t_x, float t_y, float t_z)
        : angle(t_angle), x(t_x), y(t_y), z(t_z) {}
    
    void rotation::apply(const float) 
    {
        glRotatef(angle,x,y,z);
    }



    scale::scale(float t_x, float t_y, float t_z) 
        : x(t_x), y(t_y), z(t_z) {}
    
    void scale::apply(const float) 
    {
        glScalef(x,y,z);
    }



    translation_timed::translation_timed(const float t_time_sec, const std::vector<vec3>& t_positions, bool t_closed)
        : positions(t_positions), time_sec(t_time_sec), closed(t_closed), pos_time_sec(0)
    {
        // Generate trajectory vbo
        const uint divisions = 50;
        std::vector<vec3> trajectory(divisions);
        const float step_time = time_sec / divisions;

        for(uint i = 0 ; i < divisions ; ++i)
            trajectory[i] =  catmul_rom(positions,step_time*i * ((positions.size() - !closed) / float(time_sec)) , closed);

        vbo = cg::vbo(trajectory);
    }

    void translation_timed::apply(const float delta_time_sec) 
    {   
        if(cg::scene::debug)
        {
            //cg::material::solid_color(cg::color(1,1,1));
            vbo.draw(GL_LINE_LOOP,0,-1,cg::color(1,1,1));
        }
            
        pos_time_sec += delta_time_sec;

        while(pos_time_sec > time_sec)
        {
            pos_time_sec -= time_sec;
        }
        const vec3 pos = catmul_rom(positions,pos_time_sec * ((positions.size()- !closed) / float(time_sec)) , closed);
        glTranslatef(pos[0],pos[1],pos[2]);

    }



    rotation_timed::rotation_timed(const float t_time_sec, const float t_x, const float t_y, const float t_z)
        : x(t_x) , y(t_y) , z(t_z), time_sec(t_time_sec), pos_time_sec(0) { }

    void rotation_timed::apply(const float delta_time_sec)
    {
        pos_time_sec += delta_time_sec;

        while(pos_time_sec > time_sec)
        {
            pos_time_sec -= time_sec;
        }

        glRotatef(pos_time_sec * (360.f / float(time_sec)),x,y,z);
    }



    transformation::transformation(const translation& t)
        : value(t) {}

    transformation::transformation(const rotation& r)
        : value(r) {}
    
    transformation::transformation(const scale& s)
        : value(s) {}

    transformation::transformation(const translation_timed& tt)
        : value(tt) {}

    transformation::transformation(const rotation_timed& rt)
        : value(rt) {}

    void transformation::apply(const float delta_time_sec) 
    {
        std::visit([&delta_time_sec](auto&& arg)
        {
            arg.apply(delta_time_sec);
        },value);
    }
}