#include "renderer/scene.hpp"
#include "renderer/transformation.hpp"
#include "renderer/material.hpp"
#include "generator/gen_parser.hpp"

#include <GL/glew.h>
#ifdef __APPLE__
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

namespace cg
{
    float scene::frustum[6][4];

    cg::vbo ref_vbo;
    
    void init_referential(void)
    {
        std::vector<cg::vec3> vertices;
        // X axis
        vertices.emplace_back(cg::vec3{0.0f,0.0f,0.0f});
        vertices.emplace_back(cg::vec3{1.0f,0.0f,0.0f});

        // Y axis
        vertices.emplace_back(cg::vec3{0.0f,0.0f,0.0f});
        vertices.emplace_back(cg::vec3{0.0f,1.0f,0.0f});
        
        // Z axis
        vertices.emplace_back(cg::vec3{0.0f,0.0f,0.0f});
        vertices.emplace_back(cg::vec3{0.0f,0.0f,1.0f});

        ref_vbo = cg::vbo(vertices);
    }
    void draw_referential(void)
    {
        ref_vbo.draw(GL_LINES,0,2,cg::color{1.0f, 0.0f, 0.0f, 1.0f});
        ref_vbo.draw(GL_LINES,2,2,cg::color{0.0f, 1.0f, 0.0f, 1.0f});
        ref_vbo.draw(GL_LINES,4,2,cg::color{0.0f, 0.0f, 1.0f, 1.0f});
    }

    cg::vbo grid_vbo;

    void init_grid(const float width = 20, const int divisions = 100)
    {
        std::vector<cg::vec3> vertices;
        
        const float half_width = width / 2;

        for(int i = 0 ; i <= divisions ; ++i)
    	{
    		const float xz_it = -half_width + i*(width / divisions);

            vertices.emplace_back(cg::vec3{half_width,0.0f,xz_it});
            vertices.emplace_back(cg::vec3{-half_width,0.0f,xz_it});
            vertices.emplace_back(cg::vec3{xz_it,0.0f,half_width});
            vertices.emplace_back(cg::vec3{xz_it,0.0f,-half_width});
    	}

        grid_vbo = cg::vbo(vertices);
    }
    void draw_grid(void)
    {
        grid_vbo.draw(GL_LINES,0,-1,cg::color{0.2f, 0.2f, 0.2f, 1.0f});
    }


    cg::vbo focus_vbo;

    void init_focus(void)
    {
        focus_vbo = cg::vbo(std::get<0>(gen_by_command("box 0.1 0.1 0.1 0")));
    }
    void draw_focus(cg::vec3 cam_focus_pos)
    {
        glPushMatrix();
        glTranslatef(cam_focus_pos[0],cam_focus_pos[1],cam_focus_pos[2]);
        focus_vbo.draw();
        glPopMatrix();
    }


    ////////////////////////////////////////////////////////
    // TESTING

    void extract_frustum(void)
    {
        float   proj[16];
        float   modl[16];
        float   clip[16];
        float   t;
     
        // Get the current PROJECTION matrix from OpenGL
        glGetFloatv( GL_PROJECTION_MATRIX, proj );
     
        // Get the current MODELVIEW matrix from OpenGL
        glGetFloatv( GL_MODELVIEW_MATRIX, modl );
        // Combine the two matrices (multiply projection by modelview)
        clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
        clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
        clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
        clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];
     
        clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
        clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
        clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
        clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];
     
        clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
        clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
        clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
        clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];
     
        clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
        clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
        clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
        clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];
    
        // Extract the numbers for the RIGHT plane
        scene::frustum[0][0] = clip[ 3] - clip[ 0];
        scene::frustum[0][1] = clip[ 7] - clip[ 4];
        scene::frustum[0][2] = clip[11] - clip[ 8];
        scene::frustum[0][3] = clip[15] - clip[12];
    
        // Normalize the result
        t = sqrt( scene::frustum[0][0] * scene::frustum[0][0] + scene::frustum[0][1] * scene::frustum[0][1] + scene::frustum[0][2] * scene::frustum[0][2] );
        scene::frustum[0][0] /= t;
        scene::frustum[0][1] /= t;
        scene::frustum[0][2] /= t;
        scene::frustum[0][3] /= t;
     
        // Extract the numbers for the LEFT plane
        scene::frustum[1][0] = clip[ 3] + clip[ 0];
        scene::frustum[1][1] = clip[ 7] + clip[ 4];
        scene::frustum[1][2] = clip[11] + clip[ 8];
        scene::frustum[1][3] = clip[15] + clip[12];
     
        // Normalize the result
        t = sqrt( scene::frustum[1][0] * scene::frustum[1][0] + scene::frustum[1][1] * scene::frustum[1][1] + scene::frustum[1][2] * scene::frustum[1][2] );
        scene::frustum[1][0] /= t;
        scene::frustum[1][1] /= t;
        scene::frustum[1][2] /= t;
        scene::frustum[1][3] /= t;
     
        // Extract the BOTTOM plane 
        scene::frustum[2][0] = clip[ 3] + clip[ 1];
        scene::frustum[2][1] = clip[ 7] + clip[ 5];
        scene::frustum[2][2] = clip[11] + clip[ 9];
        scene::frustum[2][3] = clip[15] + clip[13];
     
        // Normalize the result
        t = sqrt( scene::frustum[2][0] * scene::frustum[2][0] + scene::frustum[2][1] * scene::frustum[2][1] + scene::frustum[2][2] * scene::frustum[2][2] );
        scene::frustum[2][0] /= t;
        scene::frustum[2][1] /= t;
        scene::frustum[2][2] /= t;
        scene::frustum[2][3] /= t;
     
        // Extract the TOP plane
        scene::frustum[3][0] = clip[ 3] - clip[ 1];
        scene::frustum[3][1] = clip[ 7] - clip[ 5];
        scene::frustum[3][2] = clip[11] - clip[ 9];
        scene::frustum[3][3] = clip[15] - clip[13];
     
        // Normalize the result
        t = sqrt( scene::frustum[3][0] * scene::frustum[3][0] + scene::frustum[3][1] * scene::frustum[3][1] + scene::frustum[3][2] * scene::frustum[3][2] );
        scene::frustum[3][0] /= t;
        scene::frustum[3][1] /= t;
        scene::frustum[3][2] /= t;
        scene::frustum[3][3] /= t;
     
        // Extract the FAR plane
        scene::frustum[4][0] = clip[ 3] - clip[ 2];
        scene::frustum[4][1] = clip[ 7] - clip[ 6];
        scene::frustum[4][2] = clip[11] - clip[10];
        scene::frustum[4][3] = clip[15] - clip[14];
     
        // Normalize the result
        t = sqrt( scene::frustum[4][0] * scene::frustum[4][0] + scene::frustum[4][1] * scene::frustum[4][1] + scene::frustum[4][2] * scene::frustum[4][2] );
        scene::frustum[4][0] /= t;
        scene::frustum[4][1] /= t;
        scene::frustum[4][2] /= t;
        scene::frustum[4][3] /= t;
     
        // Extract the NEAR plane
        scene::frustum[5][0] = clip[ 3] + clip[ 2];
        scene::frustum[5][1] = clip[ 7] + clip[ 6];
        scene::frustum[5][2] = clip[11] + clip[10];
        scene::frustum[5][3] = clip[15] + clip[14];
     
        // Normalize the result
        t = sqrt( scene::frustum[5][0] * scene::frustum[5][0] + scene::frustum[5][1] * scene::frustum[5][1] + scene::frustum[5][2] * scene::frustum[5][2] );
        scene::frustum[5][0] /= t;
        scene::frustum[5][1] /= t;
        scene::frustum[5][2] /= t;
        scene::frustum[5][3] /= t;
    }

    /* bool mesh_in_frustum(std::vector<cg::vec3> pointlist)
    {
        for(size_t f = 0; f < 6; f++ )
        {
            size_t p;
            for(p = 0; p < pointlist.size(); p++ )
            {
                if( scene::frustum[f][0] * pointlist[p][0] + scene::frustum[f][1] * pointlist[p][1] + scene::frustum[f][2] * pointlist[p][2] + scene::frustum[f][3] > 0 )
                    break;
            }
            if( p == pointlist.size() )
                return false;
        }
        return true;
    } */


    bool boundingbox_in_frustum(const std::array<cg::vec4,8>& pointlist)
    {
        for(size_t f = 0; f < 6; f++ )
        {
            size_t p;
            for(p = 0; p < 8; p++ )
            {
                if( scene::frustum[f][0] * pointlist[p][0] + scene::frustum[f][1] * pointlist[p][1] + scene::frustum[f][2] * pointlist[p][2] + scene::frustum[f][3] > 0 )
                    break;
            }
            if( p == 8 )
                return false;
        }
        return true;
    }



    /* cg::vbo test_vbo;
    std::vector<cg::vec3> test;
    void init_test()
    {
        test.emplace_back( cg::vec3{  0.5 ,  0.5 ,  0.5 });
        test.emplace_back( cg::vec3{  0.5 ,  0.5 , -0.5 });
        test.emplace_back( cg::vec3{  0.5 , -0.5 ,  0.5 });
        test.emplace_back( cg::vec3{  0.5 , -0.5 , -0.5 });

        test.emplace_back( cg::vec3{ -0.5 ,  0.5 ,  0.5 });
        test.emplace_back( cg::vec3{ -0.5 ,  0.5 , -0.5 });
        test.emplace_back( cg::vec3{ -0.5 , -0.5 ,  0.5 });
        test.emplace_back( cg::vec3{ -0.5 , -0.5 , -0.5 });

        test_vbo = cg::vbo(std::get<0>(gen_by_command("box 1 1 1 0")));
    }
    void draw_test(void)
    {
        if(mesh_in_frustum(test))
        {
            std::cout << "Drawing\n";
            glPushMatrix();
            test_vbo.draw();
            glPopMatrix();
        }
    } */

    ////////////////////////////////////////////////////////


    int delta_time()
    {
        static int old_time_milis = 0;
        int new_time_milis = glutGet(GLUT_ELAPSED_TIME);
        int delta_time_milis = new_time_milis - old_time_milis;
        old_time_milis = new_time_milis;
        return delta_time_milis;
    }


    void enable_lights(const cg::lights& ls)
    {
        for(uint i = 0 ; i < MAX_LIGHTS ; ++i)
            if(ls[i] != nullptr)
                glEnable(cg::GL_LIGHT[i]);
    }

    //scene::scene() : models() , camera(5,5,0) { std::cout << "> scene ctor called\n"; }
    //scene::scene(const cg::group& g, const cg::camera& c) : group(g), camera(c) {}

    scene::scene(const std::vector<cg::group>& gs, cg::lights& ls, const cg::camera& c)
        : groups(gs), skybox(std::nullopt), lights(std::move(ls)), camera(c)
    {
        enable_lights(lights);
        // Referential vbo init
        init_referential();
        init_grid(150,150);
        init_focus();

        //init_test();
    }


    scene::scene(const std::vector<cg::group>& gs, const cg::model& sb, const cg::camera& c)
        : groups(gs), skybox(std::make_optional<>(sb)), camera(c)
    { 
        enable_lights(lights);
        // Referential vbo init
        init_referential();
        init_grid(150,150);
        init_focus();

        //init_test();
        
    }


    scene::scene(const std::vector<cg::group>& gs, const cg::model& sb, cg::lights& ls, const cg::camera& c)
         : groups(gs), skybox(std::make_optional<>(sb)), lights(std::move(ls)), camera(c)
    {
        enable_lights(lights);
        // Referential vbo init
        init_referential();
        init_grid(150,150);
        init_focus();
    
        //init_test();
    }
    



    void scene::compose()
    {
        camera.place();

        extract_frustum();

        //point_light(cg::vec3{5,4,0},0).apply(GL_LIGHT[0]);
        
        for(uint i = 0 ; i < MAX_LIGHTS ; ++i)
        {
            //std::cout << "gonna test\n";
            //std::cout << (lights[i] == nullptr) << "\n";
            if(lights[i] != nullptr)
            {
                (lights[i])->apply(GL_LIGHT[i]);
            }
        }

        if(skybox != std::nullopt)
        {
            skybox.value().draw();
        }


        if(debug)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            draw_referential();
            draw_grid();
            draw_focus(camera.get_direction());
        }

        float delta_time_sec = (delta_time() / 1000.f)*relative_time_multiplier;

        for(auto& g : groups)
        {
            g.draw(delta_time_sec);
        }
        
    }


    std::vector<cg::group>& scene::get_groups()
    {
        return groups;
    }

    /* cg::group& scene::get_group()
    {
        return group;
    } */

    cg::camera& scene::get_camera()
    {
        return camera;
    }

    void scene::set_camera(const cg::camera& c)
    {
        camera = c;
    }


    // ======================================== //
}