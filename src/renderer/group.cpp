#include "renderer/group.hpp"

#ifdef __APPLE__
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <iostream>

namespace cg
{
    void group::add_transformation(const transformation& t)
    {
        transformations.push_back(t);
    }

    void group::add_model(const model& m)
    {
        
        models.push_back(m);
    }

    void group::add_group(const group& g)
    {
        groups.push_back(g);
    }


    void group::draw(const float delta_time_sec)
    {
        glPushMatrix();

        glBindTexture(GL_TEXTURE_2D, 0);
        
        for(auto& t : transformations)
        {
            t.apply(delta_time_sec);
        }

        for(const auto& m : models)
        {
            m.draw();
        }
 
        for(auto& g : groups)
        {
            g.draw(delta_time_sec);
        }

        glPopMatrix();
    }

    void group::print() const
    {
        /* for(const auto& m : models)
        {
            m.print();
        } */
    }
}