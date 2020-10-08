#include "renderer/model.hpp"

#include "common/vertex.hpp"
#include "renderer/color.hpp"
#include "renderer/material.hpp"

#ifdef __APPLE__
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <IL/il.h>
#include <iostream>

#include "renderer/scene.hpp"


namespace cg 
{
	//////////////////////// PRE DEFENITIONS ////////////////////////
	std::array<cg::vec4,8> transformed_boundingbox(const std::array<cg::vec4,8>& );
	/////////////////////////////////////////////////////////////////

    model::model(const cg::vnt_vectors& t_vnts)
        : vtx_nor_tex_buffers(t_vnts), tex_id(0), bounding_box(make_bounding_box(std::get<0>(t_vnts))), diffuse{0.8,0.8,0.8,1}, ambient{0.2,0.2,0.2,1}, specular{0,0,0,1}, emission{0,0,0,1}, shininess(0) {}

    model::model(const cg::vnt_vectors& t_vnts, const uint t_tex_id) 
        : vtx_nor_tex_buffers(t_vnts), tex_id(t_tex_id), bounding_box(make_bounding_box(std::get<0>(t_vnts))), diffuse{0.8,0.8,0.8,1}, ambient{0.2,0.2,0.2,1}, specular{0,0,0,1}, emission{0,0,0,1}, shininess(0) {}

	model::model(const cg::vnt_vectors& t_vnts , const uint t_tex_id, const cg::color& diff, const cg::color& amb, const cg::color& spec, const cg::color& emi, const float s)
        : vtx_nor_tex_buffers(t_vnts), tex_id(t_tex_id), bounding_box(make_bounding_box(std::get<0>(t_vnts))), diffuse(diff), ambient(amb), specular(spec), emission(emi), shininess(s) {}
    

    void model::draw() const
    {
		extract_frustum();
		//const std::array<cg::vec4,8> updated_bb = transformed_boundingbox(bounding_box);
		if(boundingbox_in_frustum(bounding_box))
		{
			//std::cout << "Drawed\n";	
			bind_texture(tex_id);
			//std::cout << "difusse: " << diffuse[0] << "," << diffuse[1] << "," << diffuse[2] << "," << diffuse[3] << "\n";
			cg::material::apply_material(diffuse,ambient,specular,emission,shininess);
			vtx_nor_tex_buffers.draw(GL_TRIANGLES,0,-1);
		}
    }


	/* std::array<cg::vec4,8> transformed_boundingbox(const std::array<cg::vec4,8>& bounding_box)
	{
		float mv_mat[16]; 
		std::array<cg::vec4,8> res;
		glGetFloatv(GL_MODELVIEW_MATRIX, mv_mat);

		for(uint c1 = 0 ; c1 < 4 ; ++c1)
		{	for(uint c2 = 0 ; c2 < 4 ; ++c2)
			{
				std::cout << mv_mat[c1*4 + c2]  << " , ";
			}	
			std::cout << "\n";
		}

		for(uint i = 0 ; i < 8 ; ++i)
		{	
			res[i] = dot(mv_mat,bounding_box[i]);
		}

		return res;
	} */


    // Texture related //////////////////////////////////
    
    uint load_texture(const std::filesystem::path& file_name)
    {
        if(!std::filesystem::exists(file_name))
        {
            std::cerr << "> File doesn't exists\n";
            return 0;
        }

	    uint t,tw,th;
	    unsigned char *texData;
	    uint texID;

	    // Iniciar o DevIL
	    ilInit();

	    ilEnable(IL_ORIGIN_SET);
	    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	    ilGenImages(1,&t);
	    ilBindImage(t);
	    ilLoadImage((ILstring)file_name.c_str());
	    tw = ilGetInteger(IL_IMAGE_WIDTH);
	    th = ilGetInteger(IL_IMAGE_HEIGHT);

	    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	    texData = ilGetData();

	    // Gerar a textura para a placa gráfica
	    glGenTextures(1,&texID);
    
	    glBindTexture(GL_TEXTURE_2D,texID);
	    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

	    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR); // GL_NEAREST // GL_LINEAR  
	    glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_NEAREST_MIPMAP_LINEAR // GL_LINEAR_MIPMAP_LINEAR
    
	    // Upload dos dados de imagem
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	    glGenerateMipmap(GL_TEXTURE_2D);

	    glBindTexture(GL_TEXTURE_2D, 0);


	    return texID;
    }

    void bind_texture(const uint tex_id)
    {
        glBindTexture(GL_TEXTURE_2D, tex_id);
    }

	std::array<cg::vec4,8> make_bounding_box(const std::vector<cg::vec3>& vtxs)
	{
		float x_min = 0, x_max = 0;
		float y_min = 0, y_max = 0;
		float z_min = 0, z_max = 0;

		if(vtxs.size() > 0)
		{
			x_min = x_max = vtxs[0][0];
			y_min = y_max = vtxs[0][1];
			z_min = z_max = vtxs[0][2];
		}

		for(const auto& v : vtxs)
		{
			if(v[0] < x_min)
				x_min = v[0];

			if(v[0] > x_max)
				x_max = v[0];

			if(v[1] < y_min)
				y_min = v[1];

			if(v[1] > y_max)
				y_max = v[1];

			if(v[2] < z_min)
				z_min = v[2];

			if(v[2] > z_max)
				z_max = v[2];
		}

		return std::array<cg::vec4,8>{
			cg::vec4{ x_max , y_max , z_max , 1.0f },
			cg::vec4{ x_max , y_max , z_min , 1.0f },
			cg::vec4{ x_max , y_min , z_max , 1.0f },
			cg::vec4{ x_max , y_min , z_min , 1.0f },

			cg::vec4{ x_min , y_max , z_max , 1.0f },
			cg::vec4{ x_min , y_max , z_min , 1.0f },
			cg::vec4{ x_min , y_min , z_max , 1.0f },
			cg::vec4{ x_min , y_min , z_min , 1.0f }
		};
	}
}