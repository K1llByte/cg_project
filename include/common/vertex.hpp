#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "common/utils.hpp"

#include <type_traits>
#include <string>
#include <array>
#include <math.h>
#include <vector>
#include <filesystem>
#include <optional>
#include <tuple>


namespace cg
{
    //using mesh = std::vector<cg::vec3>;
    
    using vec4 = std::array<float,4>;
    using vec3 = std::array<float,3>;
    using vec2 = std::array<float,2>;


    using patches = std::tuple<std::vector<std::array<float,16>>,std::vector<cg::vec3>>;

    // An alias for the vectors of VERTICES , NORMALS and TEXTURE_COORDINATES
    using vnt_vectors = std::tuple<std::vector<vec3>,std::vector<vec3>,std::vector<vec2>>;


    constexpr cg::vec3 cross_product(const cg::vec3& a, const cg::vec3& b)
    {
        return { a[1]*b[2] - a[2]*b[1] , a[2]*b[0] - a[0]*b[2] , a[0]*b[1] - a[1]*b[0] };
    }

    constexpr float modulus(const cg::vec3& v)
    {
        return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    }

    constexpr cg::vec3& normalize(cg::vec3& v)
    {
        const float mag = modulus(v);
        if(mag == 0)
        {    
            return v;
        }
        else
        {
            v[0] /= mag;
            v[1] /= mag;
            v[2] /= mag;
            return v;
        }
    }


    // Version for to inline lvalues arguments
    constexpr cg::vec3 normalize(const cg::vec3& v)
    {
        const float mag = modulus(v);
        return (mag == 0) ? v : cg::vec3{ v[0]/mag , v[1]/mag , v[2]/mag };
    }

    
    // TODO: Abstract this later
    constexpr cg::vec4 dot(const float* mat, const cg::vec4& vec)
    {
        cg::vec4 res{0, 0, 0, 0};
        // 4x4 matrix
        for(uint i = 0 ; i < 4 ; ++i)
            for(uint j = 0 ; j < 4 ; ++j)
                res[i] += mat[i*4 + j] * vec[j];
        
        return res;
    }


    //template <class T>
    //concept floating_point = std::is_floating_point_v<T>;

    //template<typename T = float>

    // class vertex
    // {
    //     private:

    //         std::array<float,3> v;
        
    //     public:
        
    //         vertex() = default;
    //         vertex(float t_x, float t_y, float t_z);
    //         vertex(const std::array<float,3>& t_v);

    //         ~vertex() = default;

    //         constexpr float get_x() const { return v[0]; }
    //         constexpr float get_y() const { return v[1]; }
    //         constexpr float get_z() const { return v[2]; }

    //         constexpr void set(const std::array<float,3>& vtx)
    //         { v = vtx; }

    //         constexpr std::array<float,3>& get()
    //         { return v; }

    //         std::string to_string() const; // Disgust

    // };


    std::optional<cg::vnt_vectors> file_to_vertices(const std::filesystem::path& file_name);

    void vertices_to_file(const cg::vnt_vectors& mesh, const std::filesystem::path& file_name);


    std::optional<patches> file_to_patches(const std::filesystem::path& file_name);
}

#endif // VERTEX_HPP