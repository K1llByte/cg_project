#include "generator/gen.hpp"

#include "common/utils.hpp"

#include <math.h>
#include <iostream>
#include <array>

constexpr void invalid_argument_range_exit(bool predicate)
{
	if(predicate)
	{
		std::cerr << "Error: Invalid Argument Range\n";
		exit(1);
	}
}


cg::vec3 bezier_surface_aux(const std::array<cg::vec3,16>& points, const cg::vec4& mat_dot_U, const cg::vec4& mat_dot_V)
{
	cg::vec3 res{}; 
	std::array<cg::vec3,4> aux; // Will contain P.(M.V)

	// Zero initialize every vec3
	for(cg::vec3& vec : aux)
		vec = cg::vec3{};
	

	for(uint c = 0 ; c < 3 ; ++c)
	{
		// P.(M.V)
		for(uint i = 0 ; i < 4 ; ++i)
			for(uint j = 0 ; j < 4 ; ++j)
				aux[i][c] += points[i*4+j][c]*mat_dot_V[j];
	
		// U.M.(P.(M.V))
		for(uint i = 0 ; i < 4 ; ++i)
			res[c] += mat_dot_U[i]*aux[i][c];
	}

	return res;
}

cg::vec3 bezier_surface(const float u, const float v, const std::array<cg::vec3,16>& points)
{
	//std::array<float,4> vecU{ u*u*u , u*u , u , 1 };

	//std::array<float,4> vecV{ v*v*v , v*v , v , 1 };

	//const std::array<float,16> mat_M{ -1 ,  3 , -3 , 1 ,
	//								  3 , -6 ,  3 , 0 ,
	//								 -3 ,  3 ,  0 , 0 ,
	//								  1 ,  0 ,  0 , 0 };

	const std::array<float,4> mat_dot_U{ -u*u*u +3*u*u -3*u +1 ,
							        	 3*u*u*u -6*u*u +3*u ,
							        	 -3*u*u*u +3*u*u ,
							             u*u*u };

	const std::array<float,4> mat_dot_V{ -v*v*v +3*v*v -3*v +1 ,
							        	 3*v*v*v -6*v*v +3*v ,
							        	 -3*v*v*v +3*v*v ,
							             v*v*v };

	return bezier_surface_aux(points,mat_dot_U,mat_dot_V);
}

cg::vec3 bezier_u_vec(const float u, const float v, const std::array<cg::vec3,16>& points)
{
	// Derivada
	const std::array<float,4> mat_dot_U { -3*u*u  +6*u -3,
										   9*u*u -12*u +3,
										  -9*u*u  +6*u ,
										  +3*u*u };

	const std::array<float,4> mat_dot_V { -v*v*v   +3*v*v -3*v +1 ,
							        	   3*v*v*v -6*v*v +3*v ,
							        	  -3*v*v*v +3*v*v ,
							               v*v*v };

	return bezier_surface_aux(points,mat_dot_U,mat_dot_V);
}

cg::vec3 bezier_v_vec(const float u, const float v, const std::array<cg::vec3,16>& points)
{
	const std::array<float,4> mat_dot_U { -u*u*u   +3*u*u -3*u +1 ,
							        	   3*u*u*u -6*u*u +3*u ,
							        	  -3*u*u*u +3*u*u ,
							               u*u*u };

	// Derivada
	const std::array<float,4> mat_dot_V{ -3*v*v  +6*v -3 ,
							        	  9*v*v -12*v +3 ,
							        	 -9*v*v  +6*v ,
							              3*v*v };

	return bezier_surface_aux(points,mat_dot_U,mat_dot_V);
}



cg::vnt_vectors gen_skybox(const float width)
{
	invalid_argument_range_exit(width <= 0);

	const size_t NUM_VTX = 36;
	std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);

	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	const float half_width = width / 2;

	const float tex_width_x = 1.0f / 4.f;
	const float tex_width_y = 1.0f / 3.f;

	// Width X
	const float wx1 = half_width;
	const float wx2 = -half_width;
	// Height X
	const float hx1 = -half_width;
	const float hx2 = half_width;

	// =========== [ X Front Face ] =========== //

	// Vertices
	vertex_list[it++] = cg::vec3{ half_width , hx1 , wx1 };
    vertex_list[it++] = cg::vec3{ half_width , hx2 , wx2 };
    vertex_list[it++] = cg::vec3{ half_width , hx1 , wx2 };

	vertex_list[it++] = cg::vec3{ half_width , hx1 , wx1 };
    vertex_list[it++] = cg::vec3{ half_width , hx2 , wx1 };
    vertex_list[it++] = cg::vec3{ half_width , hx2 , wx2 };

	// Normals
	normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };

	normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };

	// Texture_Coord
	texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 1*tex_width_y };

	texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 2*tex_width_y };

	// =========== [ X Back Face ] =========== //

	// Vertices
	vertex_list[it++] = cg::vec3{ -half_width , hx1 , wx2 };
	vertex_list[it++] = cg::vec3{ -half_width , hx2 , wx2 };
	vertex_list[it++] = cg::vec3{ -half_width , hx1 , wx1 };

    vertex_list[it++] = cg::vec3{ -half_width , hx2 , wx2 };
    vertex_list[it++] = cg::vec3{ -half_width , hx2 , wx1 };
	vertex_list[it++] = cg::vec3{ -half_width , hx1 , wx1 };

	// Normals
	normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };

	normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };

	// Texture_Coord
	texture_list[it_t++] = cg::vec2{ 3*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 3*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 4*tex_width_x , 1*tex_width_y };

	texture_list[it_t++] = cg::vec2{ 3*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 4*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 4*tex_width_x , 1*tex_width_y };


	// Width Z
	const float wz1_1 = -half_width;
	const float wz2_1 = half_width;
	// Height Z
	const float hz1_1 = -half_width;
	const float hz2_1 = half_width;

	// =========== [ Z Front Face ] =========== //

	// Vertices
	vertex_list[it++] = cg::vec3{ wz2_1 , hz1_1 , half_width };
	vertex_list[it++] = cg::vec3{ wz1_1 , hz1_1 , half_width };
    vertex_list[it++] = cg::vec3{ wz2_1 , hz2_1 , half_width };

	vertex_list[it++] = cg::vec3{ wz1_1 , hz1_1 , half_width };
    vertex_list[it++] = cg::vec3{ wz1_1 , hz2_1 , half_width };
    vertex_list[it++] = cg::vec3{ wz2_1 , hz2_1 , half_width };

	// Normals
	normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };

	normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };

	// Texture_Coord
	texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 0             , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 2*tex_width_y };

    texture_list[it_t++] = cg::vec2{ 0	           , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 0             , 2*tex_width_y };
	texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 2*tex_width_y };

	// =========== [ Z Back Face ] =========== //
			
	// Width Z
	const float wz1_2 = half_width ;
	const float wz2_2 = -half_width;
	// Height Z
	const float hz1_2 = -half_width;
	const float hz2_2 = half_width;
			
	// Vertices
	vertex_list[it++] = cg::vec3{ wz1_2 , hz1_2 , -half_width };
    vertex_list[it++] = cg::vec3{ wz2_2 , hz2_2 , -half_width };
	vertex_list[it++] = cg::vec3{ wz2_2 , hz1_2 , -half_width };

	vertex_list[it++] = cg::vec3{ wz1_2 , hz1_2 , -half_width };
    vertex_list[it++] = cg::vec3{ wz1_2 , hz2_2 , -half_width };
    vertex_list[it++] = cg::vec3{ wz2_2 , hz2_2 , -half_width };

	// Normals
	normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };

	normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };

	// Texture_Coord
	texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 3*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 3*tex_width_x , 1*tex_width_y };

	texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 3*tex_width_x , 2*tex_width_y };

	// =========== [ Y Front Face ] =========== //

	// Width Y
	const float wy1_1 = half_width;
	const float wy2_1 = -half_width;
	// Height Y
	const float hy1_1 = half_width;
	const float hy2_1 = -half_width;

	// Vertices
	vertex_list[it++] = cg::vec3{ wy1_1 , half_width , hy1_1 };
	vertex_list[it++] = cg::vec3{ wy2_1 , half_width , hy1_1 };
    vertex_list[it++] = cg::vec3{ wy2_1 , half_width , hy2_1 };

    vertex_list[it++] = cg::vec3{ wy1_1 , half_width , hy1_1 };
	vertex_list[it++] = cg::vec3{ wy2_1 , half_width , hy2_1 };
    vertex_list[it++] = cg::vec3{ wy1_1 , half_width , hy2_1 };

	// Normals
	normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };

	normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };

	// Texture_Coord
	texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 3*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 3*tex_width_y };

	texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 2*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 3*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 2*tex_width_y };
		
	// =========== [ Y Back Face ] =========== //

	// Width Y
	const float wy1_2 = -half_width;
	const float wy2_2 = half_width;
	// Height Y
	const float hy1_2 = half_width;
	const float hy2_2 = -half_width;

	// Vertices
    vertex_list[it++] = cg::vec3{ wy2_2 , -half_width , hy2_2 };
	vertex_list[it++] = cg::vec3{ wy1_2 , -half_width , hy1_2 };
	vertex_list[it++] = cg::vec3{ wy2_2 , -half_width , hy1_2 };

    vertex_list[it++] = cg::vec3{ wy2_2 , -half_width , hy2_2 };
    vertex_list[it++] = cg::vec3{ wy1_2 , -half_width , hy2_2 };
	vertex_list[it++] = cg::vec3{ wy1_2 , -half_width , hy1_2 };

	// Normals
	normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };

	normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };

	// Texture_Coord
	texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 0             };
    texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 1*tex_width_y };

	texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 1*tex_width_y };
    texture_list[it_t++] = cg::vec2{ 2*tex_width_x , 0			   };
    texture_list[it_t++] = cg::vec2{ 1*tex_width_x , 0			   };


	return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_cylinder(const float radius, const float height, const uint slices)
{
	invalid_argument_range_exit(radius <= 0 || slices < 3);
	
	const size_t NUM_VTX = 12*slices;
    std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);
    const float angle = 2*M_PI / slices;
	const float half_height = height / 2;

    const float tex_div_width = 1.0f / slices;
	//std::cout << tex_div_width << "\n";

	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	for(uint i = 0 ; i < slices ; ++i)
	{
		// Sides
		const float tmp1 = i*angle;
		const float tmp2 = (i+1)*angle;

		// Ensure the calculation is done only once
		const float r_cos_tmp1 = radius*cos(tmp1);
		const float r_sin_tmp1 = radius*sin(tmp1);
		const float r_cos_tmp2 = radius*cos(tmp2);
		const float r_sin_tmp2 = radius*sin(tmp2);

		// Vertices
		vertex_list[it++] = cg::vec3{ r_cos_tmp1 , -half_height , r_sin_tmp1 }; //A
		vertex_list[it++] = cg::vec3{ r_cos_tmp1 , half_height  , r_sin_tmp1 }; //C
		vertex_list[it++] = cg::vec3{ r_cos_tmp2 , -half_height , r_sin_tmp2 }; //B
		
		vertex_list[it++] = cg::vec3{ r_cos_tmp1 , half_height  , r_sin_tmp1 }; //C
		vertex_list[it++] = cg::vec3{ r_cos_tmp2 , half_height  , r_sin_tmp2 }; //D
		vertex_list[it++] = cg::vec3{ r_cos_tmp2 , -half_height , r_sin_tmp2 }; //B	

		// Normals
		normals_list[it_n++] = cg::vec3{ cos(tmp1) , 0 , sin(tmp1) }; //A
		normals_list[it_n++] = cg::vec3{ cos(tmp1) , 0 , sin(tmp1) }; //C
		normals_list[it_n++] = cg::vec3{ cos(tmp2) , 0 , sin(tmp2) }; //B
		
		normals_list[it_n++] = cg::vec3{ cos(tmp1) , 0 , sin(tmp1) }; //C
		normals_list[it_n++] = cg::vec3{ cos(tmp2) , 0 , sin(tmp2) }; //D
		normals_list[it_n++] = cg::vec3{ cos(tmp2) , 0 , sin(tmp2) }; //B

		// Texture_Coords
		texture_list[it_t++] = cg::vec2{ 1.f - i*tex_div_width     , 0.375f }; //A
		texture_list[it_t++] = cg::vec2{ 1.f - i*tex_div_width     , 1.0f   }; //C
		texture_list[it_t++] = cg::vec2{ 1.f - (i+1)*tex_div_width , 0.375f }; //B
		
		texture_list[it_t++] = cg::vec2{ 1.f - i*tex_div_width     , 1.0f   }; //C
		texture_list[it_t++] = cg::vec2{ 1.f - (i+1)*tex_div_width , 1.0f   }; //D
		texture_list[it_t++] = cg::vec2{ 1.f - (i+1)*tex_div_width , 0.375f }; //B		

		// Up Base ///////////////////
		
		// Vertices
		vertex_list[it++] = cg::vec3{ r_cos_tmp2 , half_height  , r_sin_tmp2 }; //B
		vertex_list[it++] = cg::vec3{ r_cos_tmp1 , half_height  , r_sin_tmp1 }; //A
		vertex_list[it++] = cg::vec3{ 0          , half_height  , 0          }; //O	

		// Normals
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //B
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //A
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //O

		// Texture_Coords
		texture_list[it_t++] = cg::vec2{ 0.1875f*sin((i+1)*angle) + 0.4375f , 0.1875f*cos((i+1)*angle) + 0.1875f }; //B
		texture_list[it_t++] = cg::vec2{ 0.1875f*sin(i*angle) + 0.4375f     , 0.1875f*cos(i*angle) + 0.1875f     }; //A
		texture_list[it_t++] = cg::vec2{ 0.4375f                           , 0.1875f                            }; //O

		// Down Base ///////////////////

		// Vertices
		vertex_list[it++] = cg::vec3{ r_cos_tmp1 , -half_height , r_sin_tmp1 }; //A
		vertex_list[it++] = cg::vec3{ r_cos_tmp2 , -half_height , r_sin_tmp2 }; //B
		vertex_list[it++] = cg::vec3{ 0          , -half_height , 0          }; //O
		
		// Normals
		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 }; //B
		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 }; //A
		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 }; //O	

		// Texture_Coords
		texture_list[it_t++] = cg::vec2{ 0.1875f*sin(i*angle) + 0.8125f     , 0.1875f*cos(i*angle) + 0.1875f     }; //B
		texture_list[it_t++] = cg::vec2{ 0.1875f*sin((i+1)*angle) + 0.8125f , 0.1875f*cos((i+1)*angle) + 0.1875f }; //A
		texture_list[it_t++] = cg::vec2{ 0.8125f                            , 0.1875f                            }; //O
	}

    return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_sphere(const float radius, const uint slices, const uint stacks)	
{
	invalid_argument_range_exit(radius <= 0 || slices < 3 || stacks < 2);

	const size_t NUM_VTX = (6*stacks-6)*slices;
	std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);
	
	const float pi_f = M_PI;
	const float stack_angle = M_PI / stacks;
	const float slices_angle = 2*M_PI / slices;

	const float tex_slice = (1.f / slices);
	const float tex_stacks = (1.f / stacks);

	using namespace coordinates;

	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	for(uint i = 0 ; i < slices ; ++i)
	{	
		// Vertices //////////////////////////////////////////////////////

		// Down Triangle
		vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , 0				    , pi_f				 }) );
		vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , (i+1)*slices_angle , pi_f - stack_angle }) );
		vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , i*slices_angle , pi_f - stack_angle     }) );

		// Top Triangle
		vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , 0			    , 0  		      }) );
		vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , i*slices_angle , stack_angle     }) );
		vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , (i+1)*slices_angle , stack_angle }) );

		// Normals //////////////////////////////////////////////////////

		// Down Triangle
		normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , 0				   , pi_f			    }) );
		normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , (i+1)*slices_angle , pi_f - stack_angle }) );
		normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , i*slices_angle     , pi_f - stack_angle }) );

		// Top Triangle
		normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , 0			   , 0               }) );
		normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , i*slices_angle , stack_angle     }) );
		normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , (i+1)*slices_angle , stack_angle }) );

		// Textures //////////////////////////////////////////////////////

		// Down Triangle
		texture_list[it_t++] = cg::vec2( { (tex_slice / 2) + i*tex_slice , 0.f        } );
		texture_list[it_t++] = cg::vec2( { (i+1)*tex_slice               , tex_stacks } );
		texture_list[it_t++] = cg::vec2( { i*tex_slice                   , tex_stacks } );

		// Top Triangle
		texture_list[it_t++] = cg::vec2( { (tex_slice / 2) + i*tex_slice , 1.f              } );
		texture_list[it_t++] = cg::vec2( { i*tex_slice                   , 1.f - tex_stacks } );
		texture_list[it_t++] = cg::vec2( { (i+1)*tex_slice               , 1.f - tex_stacks } );

		// Body Squares
		for(uint j = 1 ; j < stacks-1 ; ++j)
		{
			// Vertices //////////////////////////////////////////////////////

			vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , (i+1)*slices_angle , j*stack_angle     }) ); //C
			vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , i*slices_angle     , j*stack_angle     }) ); //B
			vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , i*slices_angle     , (j+1)*stack_angle }) ); //A

			vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , (i+1)*slices_angle , j*stack_angle     }) ); //C
			vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , i*slices_angle     , (j+1)*stack_angle }) ); //A
			vertex_list[it++] = cg::vec3( spherical::to_cartesian({ radius , (i+1)*slices_angle , (j+1)*stack_angle }) ); //D

			// Normals //////////////////////////////////////////////////////

			normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , (i+1)*slices_angle , j*stack_angle     }) ); //C
			normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , i*slices_angle     , j*stack_angle     }) ); //B
			normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , i*slices_angle     , (j+1)*stack_angle }) ); //A

			normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , (i+1)*slices_angle , j*stack_angle     }) ); //C
			normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , i*slices_angle     , (j+1)*stack_angle }) ); //A
			normals_list[it_n++] = cg::vec3( spherical::to_cartesian({ 1 , (i+1)*slices_angle , (j+1)*stack_angle }) ); //D
		
			// Textures //////////////////////////////////////////////////////

			texture_list[it_t++] = cg::vec2( { (i+1)*tex_slice , 1.f - j*tex_stacks     } ); //C
			texture_list[it_t++] = cg::vec2( { i*tex_slice     , 1.f - j*tex_stacks     } ); //B
			texture_list[it_t++] = cg::vec2( { i*tex_slice     , 1.f - (j+1)*tex_stacks } ); //A

			texture_list[it_t++] = cg::vec2( { (i+1)*tex_slice , 1.f - j*tex_stacks     } ); //C
			texture_list[it_t++] = cg::vec2( { i*tex_slice     , 1.f - (j+1)*tex_stacks } ); //A
			texture_list[it_t++] = cg::vec2( { (i+1)*tex_slice , 1.f - (j+1)*tex_stacks } ); //D
		}
		//vertex_list.emplace_back( to_cartesian({ radius , 0 , 0 }) );
	}

	return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_plane(const float width)
{
	invalid_argument_range_exit(width <= 0);

	// Vertices
    std::vector<cg::vec3> vertex_list(6);

	const float half_width = width/2;

    vertex_list[0] = cg::vec3{  half_width , 0 ,  half_width }; //A
    vertex_list[1] = cg::vec3{  half_width , 0 , -half_width }; //B
    vertex_list[2] = cg::vec3{ -half_width , 0 , -half_width }; //C
    
    vertex_list[3] = cg::vec3{  half_width , 0 ,  half_width }; //A
    vertex_list[4] = cg::vec3{ -half_width , 0 , -half_width }; //C
    vertex_list[5] = cg::vec3{ -half_width , 0 ,  half_width }; //D

	// Normals
	std::vector<cg::vec3> normals_list(6);

	normals_list[0] = cg::vec3{ 0 , 1 , 0 };
	normals_list[1] = cg::vec3{ 0 , 1 , 0 };
	normals_list[2] = cg::vec3{ 0 , 1 , 0 };

	normals_list[3] = cg::vec3{ 0 , 1 , 0 };
	normals_list[4] = cg::vec3{ 0 , 1 , 0 };
	normals_list[5] = cg::vec3{ 0 , 1 , 0 };

	// Textures
	std::vector<cg::vec2> texture_list(6);

	texture_list[0] = cg::vec2{ 0 , 0 };
	texture_list[1] = cg::vec2{ 1 , 0 };
	texture_list[2] = cg::vec2{ 1 , 1 };

	texture_list[3] = cg::vec2{ 0 , 0 };
	texture_list[4] = cg::vec2{ 1 , 1 };
	texture_list[5] = cg::vec2{ 0 , 1 };
    
    return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_box(const float width_x, const float width_y, const float width_z, const uint divisions)
{
	invalid_argument_range_exit(width_x <= 0 || width_y <= 0 || width_z <= 0);

	const size_t NUM_VTX = (divisions+1)*(divisions+1)*36;
	std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);

	const float div_width_x = width_x / (divisions+1);
	const float div_width_y = width_y / (divisions+1);
	const float div_width_z = width_z / (divisions+1);
	const float half_width_x = width_x / 2;
	const float half_width_y = width_y / 2;
	const float half_width_z = width_z / 2;

	const float tex_width_x = 1.0f / 4.f;
	const float tex_width_y = 1.0f / 3.f;

	const float tex_div_width_x = tex_width_x / (divisions+1);
	const float tex_div_width_y = tex_width_y / (divisions+1);
	
	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	for(uint i = 0 ; i < divisions+1 ; ++i)
	{
		for(uint j = 0 ; j < divisions+1 ; ++j)
		{	
			// Width X
			const float wx1 = half_width_z - div_width_z*i;
			const float wx2 = half_width_z - div_width_z*(i+1);
			// Height X
			const float hx1 = -half_width_y + div_width_y*j;
			const float hx2 = -half_width_y + div_width_y*(j+1);

			// =========== [ X Front Face ] =========== //

			// Vertices
			vertex_list[it++] = cg::vec3{ half_width_x , hx1 , wx1 };
    		vertex_list[it++] = cg::vec3{ half_width_x , hx1 , wx2 };
    		vertex_list[it++] = cg::vec3{ half_width_x , hx2 , wx2 };

			vertex_list[it++] = cg::vec3{ half_width_x , hx1 , wx1 };
    		vertex_list[it++] = cg::vec3{ half_width_x , hx2 , wx2 };
    		vertex_list[it++] = cg::vec3{ half_width_x , hx2 , wx1 };

			// Normals
			normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };

			normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ 1 , 0 , 0 };

			// Texture_Coord
			texture_list[it_t++] = cg::vec2{ 1*tex_width_x + i*tex_div_width_x     , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (i+1)*tex_div_width_x , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (i+1)*tex_div_width_x , 1*tex_width_y + (j+1)*tex_div_width_y };

			texture_list[it_t++] = cg::vec2{ 1*tex_width_x + i*tex_div_width_x     , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (i+1)*tex_div_width_x , 1*tex_width_y + (j+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + i*tex_div_width_x     , 1*tex_width_y + (j+1)*tex_div_width_y };

			// =========== [ X Back Face ] =========== //

			// Vertices
			vertex_list[it++] = cg::vec3{ -half_width_x , hx1 , wx2 };
			vertex_list[it++] = cg::vec3{ -half_width_x , hx1 , wx1 };
    		vertex_list[it++] = cg::vec3{ -half_width_x , hx2 , wx2 };

    		vertex_list[it++] = cg::vec3{ -half_width_x , hx2 , wx2 };
			vertex_list[it++] = cg::vec3{ -half_width_x , hx1 , wx1 };
    		vertex_list[it++] = cg::vec3{ -half_width_x , hx2 , wx1 };

			// Normals
			normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };

			normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };
    		normals_list[it_n++] = cg::vec3{ -1 , 0 , 0 };

			// Texture_Coord
			texture_list[it_t++] = cg::vec2{ 3*tex_width_x + (tex_width_x - (i+1)*tex_div_width_x) , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 3*tex_width_x + (tex_width_x - i*tex_div_width_x    ) , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 3*tex_width_x + (tex_width_x - (i+1)*tex_div_width_x) , 1*tex_width_y + (j+1)*tex_div_width_y };

			texture_list[it_t++] = cg::vec2{ 3*tex_width_x + (tex_width_x - (i+1)*tex_div_width_x) , 1*tex_width_y + (j+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 3*tex_width_x + (tex_width_x - i*tex_div_width_x    ) , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 3*tex_width_x + (tex_width_x - i*tex_div_width_x    ) , 1*tex_width_y + (j+1)*tex_div_width_y };


			// Width Z
			const float wz1_1 = -half_width_x + div_width_x*i;
			const float wz2_1 = -half_width_x + div_width_x*(i+1);
			// Height Z
			const float hz1_1 = -half_width_y + div_width_y*j;
			const float hz2_1 = -half_width_y + div_width_y*(j+1);

			// =========== [ Z Front Face ] =========== //

			// Vertices
			vertex_list[it++] = cg::vec3{ wz2_1 , hz1_1 , half_width_z };
    		vertex_list[it++] = cg::vec3{ wz2_1 , hz2_1 , half_width_z };
			vertex_list[it++] = cg::vec3{ wz1_1 , hz1_1 , half_width_z };

			vertex_list[it++] = cg::vec3{ wz1_1 , hz1_1 , half_width_z };
    		vertex_list[it++] = cg::vec3{ wz2_1 , hz2_1 , half_width_z };
    		vertex_list[it++] = cg::vec3{ wz1_1 , hz2_1 , half_width_z };

			// Normals
			normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };

			normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , 1 };

			// Texture_Coord
			texture_list[it_t++] = cg::vec2{ (i+1)*tex_div_width_x , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ (i+1)*tex_div_width_x , 1*tex_width_y + (j+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ i*tex_div_width_x     , 1*tex_width_y + j*tex_div_width_y     };

    		texture_list[it_t++] = cg::vec2{ i*tex_div_width_x     , 1*tex_width_y + j*tex_div_width_y     };
			texture_list[it_t++] = cg::vec2{ (i+1)*tex_div_width_x , 1*tex_width_y + (j+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ i*tex_div_width_x     , 1*tex_width_y + (j+1)*tex_div_width_y };

			// =========== [ Z Back Face ] =========== //
			
			// Width Z
			const float wz1_2 = half_width_x - div_width_x*i;
			const float wz2_2 = half_width_x - div_width_x*(i+1);
			// Height Z
			const float hz1_2 = -half_width_y + div_width_y*j;
			const float hz2_2 = -half_width_y + div_width_y*(j+1);
			
			// Vertices
			vertex_list[it++] = cg::vec3{ wz1_2 , hz1_2 , -half_width_z };
			vertex_list[it++] = cg::vec3{ wz2_2 , hz1_2 , -half_width_z };
    		vertex_list[it++] = cg::vec3{ wz2_2 , hz2_2 , -half_width_z };

			vertex_list[it++] = cg::vec3{ wz1_2 , hz1_2 , -half_width_z };
    		vertex_list[it++] = cg::vec3{ wz2_2 , hz2_2 , -half_width_z };
    		vertex_list[it++] = cg::vec3{ wz1_2 , hz2_2 , -half_width_z };

			// Normals
			normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };

			normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };
    		normals_list[it_n++] = cg::vec3{ 0 , 0 , -1 };

			// Texture_Coord
			texture_list[it_t++] = cg::vec2{ 2*tex_width_x + i*tex_div_width_x     , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 2*tex_width_x + (i+1)*tex_div_width_x , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 2*tex_width_x + (i+1)*tex_div_width_x , 1*tex_width_y + (j+1)*tex_div_width_y };

			texture_list[it_t++] = cg::vec2{ 2*tex_width_x + i*tex_div_width_x     , 1*tex_width_y + j*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 2*tex_width_x + (i+1)*tex_div_width_x , 1*tex_width_y + (j+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 2*tex_width_x + i*tex_div_width_x     , 1*tex_width_y + (j+1)*tex_div_width_y };

			// =========== [ Y Front Face ] =========== //

			// Width Y
			const float wy1_1 = half_width_x - div_width_x*i;
			const float wy2_1 = half_width_x - div_width_x*(i+1);
			// Height Y
			const float hy1_1 = half_width_z - div_width_z*j;
			const float hy2_1 = half_width_z - div_width_z*(j+1);

			// Vertices
			vertex_list[it++] = cg::vec3{ wy1_1 , half_width_y , hy1_1 };
    		vertex_list[it++] = cg::vec3{ wy2_1 , half_width_y , hy2_1 };
			vertex_list[it++] = cg::vec3{ wy2_1 , half_width_y , hy1_1 };

    		vertex_list[it++] = cg::vec3{ wy1_1 , half_width_y , hy1_1 };
    		vertex_list[it++] = cg::vec3{ wy1_1 , half_width_y , hy2_1 };
			vertex_list[it++] = cg::vec3{ wy2_1 , half_width_y , hy2_1 };

			// Normals
			normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };

			normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 };

			// Texture_Coord
			texture_list[it_t++] = cg::vec2{ 1*tex_width_x + j*tex_div_width_x     , 2*tex_width_y + i*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (j+1)*tex_div_width_x , 2*tex_width_y + (i+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + j*tex_div_width_x , 2*tex_width_y + (i+1)*tex_div_width_y     };

			texture_list[it_t++] = cg::vec2{ 1*tex_width_x + j*tex_div_width_x     , 2*tex_width_y + i*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (j+1)*tex_div_width_x     , 2*tex_width_y + i*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (j+1)*tex_div_width_x , 2*tex_width_y + (i+1)*tex_div_width_y };
		
			// =========== [ Y Back Face ] =========== //

			// Width Y
			const float wy1_2 = -half_width_x + div_width_x*i;
			const float wy2_2 = -half_width_x + div_width_x*(i+1);
			// Height Y
			const float hy1_2 = half_width_z - div_width_z*j;
			const float hy2_2 = half_width_z - div_width_z*(j+1);

			// Vertices
    		vertex_list[it++] = cg::vec3{ wy2_2 , -half_width_y , hy2_2 };
			vertex_list[it++] = cg::vec3{ wy2_2 , -half_width_y , hy1_2 };
			vertex_list[it++] = cg::vec3{ wy1_2 , -half_width_y , hy1_2 };

    		vertex_list[it++] = cg::vec3{ wy2_2 , -half_width_y , hy2_2 };
			vertex_list[it++] = cg::vec3{ wy1_2 , -half_width_y , hy1_2 };
    		vertex_list[it++] = cg::vec3{ wy1_2 , -half_width_y , hy2_2 };

			// Normals
			normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };

			normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };
    		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 };

			// Texture_Coord
			texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (j+1)*tex_div_width_x , (i+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + j*tex_div_width_x     , (i+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + j*tex_div_width_x     , i*tex_div_width_y     };

			texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (j+1)*tex_div_width_x , (i+1)*tex_div_width_y };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + j*tex_div_width_x     , i*tex_div_width_y     };
    		texture_list[it_t++] = cg::vec2{ 1*tex_width_x + (j+1)*tex_div_width_x , i*tex_div_width_y     };
		}
	}

	return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_cone(const float radius, const float height, const uint slices, const uint stacks)
{
	invalid_argument_range_exit(radius <= 0 || height < 0 || slices < 3 || stacks < 1);

	const size_t NUM_VTX = 6*slices*stacks;
	std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);
	const float angle = 2*M_PI / slices;
	const float stack_height = height / stacks;
	const float stack_radius = radius / stacks;

	const float tex_div_slice = 2*M_PI / slices;
	

	using namespace coordinates;
	
	
	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	for(uint i = 0 ; i < slices ; ++i)
	{
		const float a1 = i*angle;
		const float a2 = (i+1)*angle;

		// Top Triangle
		
		// Vertices
		vertex_list[it++] = cg::vec3{ 				    	 0 , 				height ,  0 }; //A
		vertex_list[it++] = polar::to_cartesian({ stack_radius , height - stack_height , a1 }); //C
		vertex_list[it++] = polar::to_cartesian({ stack_radius , height - stack_height , a2 }); //B

		// Normals
		normals_list[it_n++] =  polar::to_cartesian({ 1 , sin(atan(radius/height)) , (a1 + a2)/2 }); //A
		normals_list[it_n++] =  polar::to_cartesian({ 1 , sin(atan(radius/height)) ,          a1 }); //C
		normals_list[it_n++] =  polar::to_cartesian({ 1 , sin(atan(radius/height)) ,          a2 }); //B

		// Texture_Coords
		texture_list[it_t++] = cg::vec2{ 0.75f , 0.5f }; //A
		texture_list[it_t++] = cg::vec2{ 0.75f + (0.2f / stacks)*cos(    i*tex_div_slice) , 0.5f + (0.5f / stacks)*sin(    i*tex_div_slice) }; //C
		texture_list[it_t++] = cg::vec2{ 0.75f + (0.2f / stacks)*cos((i+1)*tex_div_slice) , 0.5f + (0.5f / stacks)*sin((i+1)*tex_div_slice) }; //B

		// Cone Body
		for(uint j = 0 ; j < stacks-1 ; ++j)
		{
			// Ensure the calculation is done only once
			const float r1 = radius - j*stack_radius;
			const float r2 = radius - (j+1)*stack_radius;
			const float y1 = j*stack_height;
			const float y2 = (j+1)*stack_height;

            // Vertices
			vertex_list[it++] = polar::to_cartesian({ r1 , y1 , a1 }); //A
			vertex_list[it++] = polar::to_cartesian({ r2 , y2 , a2 }); //B
			vertex_list[it++] = polar::to_cartesian({ r2 , y2 , a1 }); //C

			vertex_list[it++] = polar::to_cartesian({ r1 , y1 , a2 }); //D
			vertex_list[it++] = polar::to_cartesian({ r2 , y2 , a2 }); //B
			vertex_list[it++] = polar::to_cartesian({ r1 , y1 , a1 }); //A

            // Normals
            normals_list[it_n++] = cg::normalize( polar::to_cartesian({ 1 , sin(atan(radius/height)) , a1 }) ); //A
			normals_list[it_n++] = cg::normalize( polar::to_cartesian({ 1 , sin(atan(radius/height)) , a2 }) ); //B
			normals_list[it_n++] = cg::normalize( polar::to_cartesian({ 1 , sin(atan(radius/height)) , a1 }) ); //C

			normals_list[it_n++] = cg::normalize( polar::to_cartesian({ 1 , sin(atan(radius/height)) , a2 }) ); //D
			normals_list[it_n++] = cg::normalize( polar::to_cartesian({ 1 , sin(atan(radius/height)) , a2 }) ); //B
			normals_list[it_n++] = cg::normalize( polar::to_cartesian({ 1 , sin(atan(radius/height)) , a1 }) ); //A

            // Texture_Coords
			texture_list[it_t++] = cg::vec2{ 0.75f + (stacks-    j)*(0.25f / stacks)*cos(    i*tex_div_slice) , 0.5f + (stacks-    j)*(0.5f / stacks)*sin(    i*tex_div_slice) }; //A
			texture_list[it_t++] = cg::vec2{ 0.75f + (stacks-(j+1))*(0.25f / stacks)*cos((i+1)*tex_div_slice) , 0.5f + (stacks-(j+1))*(0.5f / stacks)*sin((i+1)*tex_div_slice) }; //C
			texture_list[it_t++] = cg::vec2{ 0.75f + (stacks-(j+1))*(0.25f / stacks)*cos(    i*tex_div_slice) , 0.5f + (stacks-(j+1))*(0.5f / stacks)*sin(    i*tex_div_slice) }; //B

			texture_list[it_t++] = cg::vec2{ 0.75f + (stacks-    j)*(0.25f / stacks)*cos((i+1)*tex_div_slice) , 0.5f + (stacks-    j)*(0.5f / stacks)*sin((i+1)*tex_div_slice) }; //D
			texture_list[it_t++] = cg::vec2{ 0.75f + (stacks-(j+1))*(0.25f / stacks)*cos((i+1)*tex_div_slice) , 0.5f + (stacks-(j+1))*(0.5f / stacks)*sin((i+1)*tex_div_slice) }; //A
			texture_list[it_t++] = cg::vec2{ 0.75f + (stacks-    j)*(0.25f / stacks)*cos(    i*tex_div_slice) , 0.5f + (stacks-    j)*(0.5f / stacks)*sin(    i*tex_div_slice) }; //B
		}

		//Base Triangle
		vertex_list[it++] = polar::to_cartesian({ radius , 0 , a2 }); //B
		vertex_list[it++] = polar::to_cartesian({ radius , 0 , a1 }); //A
		vertex_list[it++] = polar::to_cartesian({      0 , 0 ,  0 }); //O

        // Normals
		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 }; //B
		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 }; //A
		normals_list[it_n++] = cg::vec3{ 0 , -1 , 0 }; //O

		// Texture_Coords
		texture_list[it_t++] = cg::vec2{ 0.25f + 0.25f*cos(i*tex_div_slice) , 0.5f + 0.5f*sin(i*tex_div_slice) }; //A
		texture_list[it_t++] = cg::vec2{ 0.25f + 0.25f*cos((i+1)*tex_div_slice) , 0.5f + 0.5f*sin((i+1)*tex_div_slice) }; //C
		texture_list[it_t++] = cg::vec2{ 0.25f , 0.5f }; //B
	}

	return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_ring(const float inner_radius, const float outter_radius, const uint slices)
{
	invalid_argument_range_exit(outter_radius <= 0 || inner_radius <= 0 || slices < 3);

	const size_t NUM_VTX = 6*slices;

	std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);
    const float angle = 2*M_PI / slices;

	//const float tex_div_slice = 1.0f / slices;

	
	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	for(uint i = 0 ; i < slices ; ++i)
	{
		// Sides
		const float a1 = i*angle;
		const float a2 = (i+1)*angle;

		// Vertices
		vertex_list[it++] = cg::vec3{ outter_radius*cos(a1) , 0 , outter_radius*sin(a1) }; //A
		vertex_list[it++] = cg::vec3{ inner_radius*cos(a1)  , 0 , inner_radius*sin(a1)  }; //B
		vertex_list[it++] = cg::vec3{ outter_radius*cos(a2) , 0 , outter_radius*sin(a2) }; //C

		vertex_list[it++] = cg::vec3{ outter_radius*cos(a2) , 0 , outter_radius*sin(a2) }; //A
		vertex_list[it++] = cg::vec3{ inner_radius*cos(a1)  , 0 , inner_radius*sin(a1)  }; //B
		vertex_list[it++] = cg::vec3{ inner_radius*cos(a2)  , 0 , inner_radius*sin(a2)  }; //C

		// Normals
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //A
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //B
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //C	

		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //A
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //B
		normals_list[it_n++] = cg::vec3{ 0 , 1 , 0 }; //C

		// Texture_Coords
		texture_list[it_t++] = cg::vec2{ 1 , 0 }; //A
		texture_list[it_t++] = cg::vec2{ 0 , 0 }; //B
		texture_list[it_t++] = cg::vec2{ 1 , 1 }; //C	

		texture_list[it_t++] = cg::vec2{ 1 , 0 }; //A
		texture_list[it_t++] = cg::vec2{ 0 , 1 }; //B
		texture_list[it_t++] = cg::vec2{ 0 , 0 }; //C
	}

	return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_torus(const float radius, const float cyl_radius, const uint slices, const uint cyl_slices)
{
	invalid_argument_range_exit(radius <= 0 || cyl_radius <= 0 || cyl_radius >= radius || slices < 3 || cyl_slices < 3);

	const size_t NUM_VTX = 6*slices*cyl_slices;

	std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);
    const float angle = 2*M_PI / slices;
	const float cyl_angle = 2*M_PI / cyl_slices;
	const float cyl_distance = radius - cyl_radius;

	const float tex_div_slice = 1.0f / slices;
	const float tex_div_cyl_slice = 1.0f / cyl_slices;

	
	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	for(uint i = 0 ; i < slices ; ++i)
	{
		// Sides
		const float a1 = i*angle;
		const float a2 = (i+1)*angle;

		for(uint j = 0 ; j < cyl_slices ; ++j)
		{
			const float ca1 = j*cyl_angle;
			const float ca2 = (j+1)*cyl_angle;

			// Vertices
			vertex_list[it++] = cg::vec3{ (cyl_distance + cyl_radius*cos(ca1))*cos(a1) , cyl_radius*sin(ca1) , (cyl_distance + cyl_radius*cos(ca1))*sin(a1) }; //B
			vertex_list[it++] = cg::vec3{ (cyl_distance + cyl_radius*cos(ca2))*cos(a1) , cyl_radius*sin(ca2) , (cyl_distance + cyl_radius*cos(ca2))*sin(a1) }; //C
			vertex_list[it++] = cg::vec3{ (cyl_distance + cyl_radius*cos(ca1))*cos(a2) , cyl_radius*sin(ca1) , (cyl_distance + cyl_radius*cos(ca1))*sin(a2) }; //A	

			vertex_list[it++] = cg::vec3{ (cyl_distance + cyl_radius*cos(ca1))*cos(a2) , cyl_radius*sin(ca1) , (cyl_distance + cyl_radius*cos(ca1))*sin(a2) }; //A
			vertex_list[it++] = cg::vec3{ (cyl_distance + cyl_radius*cos(ca2))*cos(a1) , cyl_radius*sin(ca2) , (cyl_distance + cyl_radius*cos(ca2))*sin(a1) }; //C
			vertex_list[it++] = cg::vec3{ (cyl_distance + cyl_radius*cos(ca2))*cos(a2) , cyl_radius*sin(ca2) , (cyl_distance + cyl_radius*cos(ca2))*sin(a2) }; //B

			// Normals
			normals_list[it_n++] = cg::vec3{ cos(ca1)*cos(a1) , sin(ca1) , cos(ca1)*sin(a1) }; //B
			normals_list[it_n++] = cg::vec3{ cos(ca2)*cos(a1) , sin(ca2) , cos(ca2)*sin(a1) }; //C
			normals_list[it_n++] = cg::vec3{ cos(ca1)*cos(a2) , sin(ca1) , cos(ca1)*sin(a2) }; //A	

			normals_list[it_n++] = cg::vec3{ cos(ca1)*cos(a2) , sin(ca1) , cos(ca1)*sin(a2) }; //A
			normals_list[it_n++] = cg::vec3{ cos(ca2)*cos(a1) , sin(ca2) , cos(ca2)*sin(a1) }; //C
			normals_list[it_n++] = cg::vec3{ cos(ca2)*cos(a2) , sin(ca2) , cos(ca2)*sin(a2) }; //B

			// Texture_Coords
			texture_list[it_t++] = cg::vec2{     j*tex_div_cyl_slice ,     i*tex_div_slice }; //B
			texture_list[it_t++] = cg::vec2{ (j+1)*tex_div_cyl_slice ,     i*tex_div_slice }; //C
			texture_list[it_t++] = cg::vec2{     j*tex_div_cyl_slice , (i+1)*tex_div_slice }; //A	

			texture_list[it_t++] = cg::vec2{     j*tex_div_cyl_slice , (i+1)*tex_div_slice }; //A
			texture_list[it_t++] = cg::vec2{ (j+1)*tex_div_cyl_slice ,     i*tex_div_slice }; //C
			texture_list[it_t++] = cg::vec2{ (j+1)*tex_div_cyl_slice , (i+1)*tex_div_slice }; //B
		}

		
		
	}

	return std::make_tuple(vertex_list,normals_list,texture_list);
}


cg::vnt_vectors gen_patches(const std::vector<std::array<float,16>>& patches_indices, const std::vector<cg::vec3>& control_points, const uint num_divisions)
{
	invalid_argument_range_exit(num_divisions < 1);

	const size_t NUM_VTX = 6*num_divisions*num_divisions*patches_indices.size();

	std::vector<cg::vec3> vertex_list(NUM_VTX);
	std::vector<cg::vec3> normals_list(NUM_VTX);
	std::vector<cg::vec2> texture_list(NUM_VTX);
	
	const float d_value = 1.0f / num_divisions;

	uint it = 0;
	uint it_n = 0;
	uint it_t = 0;

	for(std::array<float,16> idxs : patches_indices)
	{
		std::array<cg::vec3,16> idx_to_mat{ 
			control_points[idxs[0]]  , control_points[idxs[1]]  , control_points[idxs[2]]  , control_points[idxs[3]]  ,
			control_points[idxs[4]]  , control_points[idxs[5]]  , control_points[idxs[6]]  , control_points[idxs[7]]  ,
			control_points[idxs[8]]  , control_points[idxs[9]]  , control_points[idxs[10]] , control_points[idxs[11]] ,
			control_points[idxs[12]] , control_points[idxs[13]] , control_points[idxs[14]] , control_points[idxs[15]] };

		for(uint u = 0 ; u < num_divisions; u++)
		{
			for(uint v = 0 ; v < num_divisions ; v++)
			{
				const float u1 = u*d_value;
				const float u2 = (u+1)*d_value;
				const float v1 = v*d_value;
				const float v2 = (v+1)*d_value;
				// Vertices
				vertex_list[it++] = bezier_surface( u1 , v1 , idx_to_mat );
				vertex_list[it++] = bezier_surface( u1 , v2 , idx_to_mat );
				vertex_list[it++] = bezier_surface( u2 , v1 , idx_to_mat );

				vertex_list[it++] = bezier_surface( u1 , v2 , idx_to_mat );
				vertex_list[it++] = bezier_surface( u2 , v2 , idx_to_mat );
				vertex_list[it++] = bezier_surface( u2 , v1 , idx_to_mat );

				// Normals
				normals_list[it_n++] = cg::normalize( cg::cross_product( bezier_v_vec(u1, v1, idx_to_mat) , bezier_u_vec(u1, v1, idx_to_mat) ) );
				normals_list[it_n++] = cg::normalize( cg::cross_product( bezier_v_vec(u1, v2, idx_to_mat) , bezier_u_vec(u1, v2, idx_to_mat) ) );
				normals_list[it_n++] = cg::normalize( cg::cross_product( bezier_v_vec(u2, v1, idx_to_mat) , bezier_u_vec(u2, v1, idx_to_mat) ) );

				normals_list[it_n++] = cg::normalize( cg::cross_product( bezier_v_vec(u1, v2, idx_to_mat) , bezier_u_vec(u1, v2, idx_to_mat) ) );
				normals_list[it_n++] = cg::normalize( cg::cross_product( bezier_v_vec(u2, v2, idx_to_mat) , bezier_u_vec(u2, v2, idx_to_mat) ) );
				normals_list[it_n++] = cg::normalize( cg::cross_product( bezier_v_vec(u2, v1, idx_to_mat) , bezier_u_vec(u2, v1, idx_to_mat) ) );

				// Texture_Coords
				texture_list[it_t++] = cg::vec2{ u1 , v1 };
				texture_list[it_t++] = cg::vec2{ u1 , v2 };
				texture_list[it_t++] = cg::vec2{ u2 , v1 };

				texture_list[it_t++] = cg::vec2{ u1 , v2 };
				texture_list[it_t++] = cg::vec2{ u2 , v2 };
				texture_list[it_t++] = cg::vec2{ u2 , v1 };
			}
		}
	}

	return std::make_tuple(vertex_list,normals_list,texture_list);
}