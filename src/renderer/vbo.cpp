#include "renderer/vbo.hpp"

#include "renderer/material.hpp"


namespace cg
{
    vbo::vbo(const std::vector<vec3>& vertices)
    {
        glGenBuffers(1,&vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        num_vertices = vertices.size();
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*3 * sizeof(float), (float*) vertices.data(), GL_STATIC_DRAW);
    }

    void vbo::draw(const GLenum mode, const uint first, int count, const cg::color& clr) const
    {
        //std::cout << "entered\n";
        if(count == -1)
        {
            count = num_vertices;
        }

        glBindBuffer(GL_ARRAY_BUFFER,vbo_id);
        glVertexPointer(3,GL_FLOAT,0, 0);

        if(clr[0] < 0 || clr[1] < 0 || clr[2] < 0 || clr[3] < 0)
        {
            auto& tmp = rgb();
            //glColor4f(tmp[0],tmp[1],tmp[2],1.0f);
            cg::material::solid_color(cg::color{tmp[0],tmp[1],tmp[2]});
        }
        else
            cg::material::solid_color(clr);
            //glColor4f(clr[0],clr[1],clr[2],clr[3]);
        glDrawArrays(mode, first, count);
        //std::cout << "exited\n";
    }




    vnt_bo::vnt_bo(const cg::vnt_vectors& vnt_s)
    {
        num_vnt = std::get<0>(vnt_s).size();

        glGenBuffers(1, &vertices_id);
	    glBindBuffer(GL_ARRAY_BUFFER,vertices_id);
	    glBufferData(GL_ARRAY_BUFFER, num_vnt*3 * sizeof(float), std::get<0>(vnt_s).data(), GL_STATIC_DRAW);

	    glGenBuffers(1, &normals_id);
	    glBindBuffer(GL_ARRAY_BUFFER,normals_id);
	    glBufferData(GL_ARRAY_BUFFER, num_vnt*3 * sizeof(float), std::get<1>(vnt_s).data(), GL_STATIC_DRAW);

	    glGenBuffers(1, &textures_id);
	    glBindBuffer(GL_ARRAY_BUFFER, textures_id);
	    glBufferData(GL_ARRAY_BUFFER, num_vnt*2 * sizeof(float), std::get<2>(vnt_s).data(), GL_STATIC_DRAW);


    }

    void vnt_bo::draw(const GLenum mode, const uint first, int count) const
    {
        if(count == -1)
        {
            count = num_vnt;
        }

        glBindBuffer(GL_ARRAY_BUFFER,vertices_id);
	    glVertexPointer(3,GL_FLOAT,0,0);

	    glBindBuffer(GL_ARRAY_BUFFER,normals_id);
	    glNormalPointer(GL_FLOAT,0,0);
	
	    glBindBuffer(GL_ARRAY_BUFFER,textures_id);
	    glTexCoordPointer(2,GL_FLOAT,0,0);

        //if(clr[0] < 0 || clr[1] < 0 || clr[2] < 0 || clr[3] < 0)
        //{
        //    auto& tmp = rgb(40/255.f);
        //    //glColor4f(tmp[0],tmp[1],tmp[2],1.0f);
        //    cg::material::solid_color(cg::color{tmp[0],tmp[1],tmp[2]});
        //}
        //else
        //    cg::material::solid_color(clr);

	    glDrawArrays(mode, first, count);
    }
}