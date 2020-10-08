#ifndef GEN_HPP
#define GEN_HPP

#include "common/vertex.hpp"

#include <vector>

cg::vnt_vectors gen_skybox(const float width);

cg::vnt_vectors gen_cylinder(const float radius, const float height, const uint slices);

cg::vnt_vectors gen_sphere(const float radius, const uint slices, const uint stacks);

cg::vnt_vectors gen_plane(const float width);

cg::vnt_vectors gen_box(const float width_x, const float width_y, const float width_z, const uint divisions);

cg::vnt_vectors gen_cone(const float radius, const float height, const uint slices, const uint stacks);

cg::vnt_vectors gen_ring(const float inner_radius, const float outter_radius, const uint slices);

cg::vnt_vectors gen_torus(const float radius, const float cyl_radius, const uint slices, const uint cyl_slices);

cg::vnt_vectors gen_patches(const std::vector<std::array<float,16>>& patches_indices, const std::vector<cg::vec3>& control_points, const uint num_divisions = 5);

#endif // GEN_HPP