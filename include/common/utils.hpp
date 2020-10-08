#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>
#include <math.h>
#include <string_view>
#include <string>
#include <vector>

// Define the DEBUG_TOOLS label to use this macros
#ifdef DEBUG_TOOLS
#   define LOG(to_print) std::cout << to_print << '\n'
#   define DEBUG std::cout << "OK!\n";
#endif




namespace str_utils
{
    std::vector<std::string_view> split(const std::string& cmd, const char separator = ' ');
}
    



namespace coordinates
{
    namespace spherical
    {
        constexpr std::array<float,3> to_cartesian(const std::array<float,3>& v)
        {
            const float s_p = v[0];
            const float s_theta = v[1];
            const float s_phi = v[2];

            std::array<float,3> new_vec
                { sin(s_theta) * sin(s_phi) * s_p , cos(s_phi) * s_p , sin(s_phi) * s_p * cos(s_theta) };

            return new_vec;
        }


        constexpr std::array<float,3> to_spherical(const std::array<float,3>& v)
        {
            const float x = v[0];
            const float y = v[1];
            const float z = v[2];

            std::array<float,3> new_vec
                { sqrt(x*x + y*y + z*z) , atan2(x,z) , acos(y / sqrt(x*x + y*y + z*z)) };

            return new_vec;
        }
    }


    namespace polar
    {
        constexpr std::array<float,3> to_cartesian(const std::array<float,3>& v)
        {
            const float s_radius = v[0];
            const float y = v[1];
            const float s_theta = v[2];

            std::array<float,3> new_vec
                { s_radius*sin(s_theta) , y , s_radius*cos(s_theta) };

            return new_vec;
        }


        constexpr std::array<float,3> to_polar(const std::array<float,3>& v)
        {
            const float x = v[0];
            const float y = v[1];
            const float z = v[2];

            std::array<float,3> new_vec
                { sqrt(x*x + z*z) , y , atan2(x,z) };

            return new_vec;
        }
    }
}

/* constexpr std::array<float,3> to_cartesian(const std::array<float,3>& v)
{
    const float s_p = v[0];
    const float s_theta = v[1];
    const float s_phi = v[2];

    std::array<float,3> new_vec
        { sin(s_theta) * sin(s_phi) * s_p , cos(s_phi) * s_p , sin(s_phi) * s_p * cos(s_theta) };

    return new_vec;
}

constexpr std::array<float,3> to_spherical(const std::array<float,3>& v)
{
    const float x = v[0];
    const float y = v[1];
    const float z = v[2];

    std::array<float,3> new_vec
        { sqrt(x*x + y*y + z*z) , atan2(x,z) , acos(y / sqrt(x*x + y*y + z*z)) };

    return new_vec;
}



constexpr std::array<float,3> to_polar(const std::array<float,3>& v)
{
    const float x = v[0];
    const float y = v[1];
    const float z = v[2];

    std::array<float,3> new_vec
        { sqrt(x*x + y*y + z*z) , atan2(x,z) , acos(y / sqrt(x*x + y*y + z*z)) };

    return new_vec;
}


constexpr std::array<float,3> to_cartesian(const std::array<float,3>& v)
{
    const float x = v[0];
    const float y = v[1];
    const float z = v[2];

    std::array<float,3> new_vec
        { sqrt(x*x + y*y + z*z) , atan2(x,z) , acos(y / sqrt(x*x + y*y + z*z)) };

    return new_vec;
}
 */


/* 
constexpr std::array<float,3>& to_cartesian(const std::array<float,3>& v)
{
    const float s_p = v[0];
    const float s_theta = v[1];
    const float s_phi = v[2];

    v[0] = sin(s_theta) * sin(s_phi) * s_p;
	v[1] = cos(s_phi) * s_p;
	v[2] = sin(s_phi) * s_p * cos(s_theta);

    return v;
}

constexpr std::array<float,3>& to_spherical(const std::array<float,3>& v)
{
    const float x = v[0];
    const float y = v[1];
    const float z = v[2];
    //std::array<float,3>();
    v[0] = sqrt(x*x + y*y + z*z);
    v[1] = atan2(x,z);
    v[2] = acos(y / v[0]);

    return v;
}
 */
#endif // UTILS_HPP