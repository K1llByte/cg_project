#include "renderer/color.hpp"

#include <iostream>
#include <ctime>
#include <chrono>

namespace cg
{
    color::color() : rgba{ 1.f , 1.f , 1.f , 1.f } {}
    color::color(float t_r, float t_g, float t_b) : rgba{ t_r, t_g, t_b, 1.f } {}
    color::color(float t_r, float t_g, float t_b, float t_a) : rgba{ t_r, t_g, t_b, t_a } {}
    color::color(const std::array<float,3>& t_v) : rgba{ t_v[0] , t_v[1] , t_v[2] , 1.f} {}
    color::color(const std::array<float,4>& t_v) : rgba{ t_v[0] , t_v[1] , t_v[2] , t_v[3]} {}
    //color::color(const bool& rgb) : rgba{1.f} , rgb{rgb} {}
    //color::color(const color& t_clr) : /* rgba(t_clr.rgba) */rgba(t_clr.data()) {}


    float color::operator[](char channel) const
    {
        switch(channel)
        {
            case 'r':
                return rgba[0];
            case 'g':
                return rgba[1];
            case 'b':
                return rgba[2];
            case 'a':
                return rgba[3];
            default:
                return 0;
        }
    }

    float color::operator[](int pos) const
    {
        return rgba[pos];
    }

    // ================= Not From Class ================= //


    constexpr float rad_to_deg(const float rad)
    {
        //const float tmp_f = (360.f + 180.f * rad / M_PI);
        //const int tmp_i = int(tmp_f);
        //const float delta = tmp_f - tmp_i;
        //return tmp_i % 360 + delta;
        return fmod(360.f + 180.f * rad / M_PI , 360);
    }

    constexpr cg::vec3 hsv_to_rgb(float hue, float saturation, float value) 
    {
        hue /= 60;
        float chroma = value * saturation;
        float x = chroma * (1 - std::abs((fmod(hue, 2)) - 1));
        cg::vec3 rgb = (hue <= 1) ? cg::vec3{chroma, x, 0}:
                       (hue <= 2) ? cg::vec3{x, chroma, 0}:
                       (hue <= 3) ? cg::vec3{0, chroma, x}:
                       (hue <= 4) ? cg::vec3{0, x, chroma}:
                       (hue <= 5) ? cg::vec3{x, 0, chroma}:
                                 cg::vec3{chroma, 0, x};

        const float m = value - chroma;
        return cg::vec3{ rgb[0]+m , rgb[1]+m , rgb[2] + m};
    }


    const cg::vec3 new_rgb(const float angle)
    {
        const float x = cos(angle);
        const float y = sin(angle);
        const float r = std::sqrt(x*x + y*y);
        
        float sat = (r > 1) ? 0 : r;
    
        //std::cout << "aaa:" << rad_to_deg(std::atan2(y, x)) << "\n";
        //std::cout << "AAAAAAA:" << int(rad_to_deg(std::atan2(y, x))) << "\n";
        return hsv_to_rgb(rad_to_deg(std::atan2(y, x)),sat,1.0f);
    }

    const cg::vec3 rgb(const float speed)
    {
        const float angle = double(clock() / double(CLOCKS_PER_SEC)) * speed;
        //std::cout << "clock: " << angle << "\n";
        return new_rgb(angle);
    }

    /* const cg::vec3& old_rgb(const float speed)
    {
        static cg::vec3 arr{ 1 , 0 , 0 };

        static int8_t inc = 2;
        static int8_t dec = -1;

        //std::cout << "time: " << double(clock() / double(CLOCKS_PER_SEC)) << " inc:" << int(inc) << " dec:" << int(dec) << "\n";
        //static float angle_tmp = 0;
        //angle_tmp += M_PI/18.f;
        
        //std::cout << "rgb: (" << tmp[0] << "," << tmp[1] << "," << tmp[2] << ")\n";

        if(inc > 0)
        {
            //std::cout << int(inc) << ' ' << int(dec) << '\n';
            arr[inc-1] += speed;
            if(arr[inc-1] >= 1)
            {
                inc = -((inc % 3) + 1);
                dec = -dec;
            }
        }
        else
        {
            arr[dec-1] -= speed;
            if(arr[dec-1] <= 0)
            {
                dec = -((dec % 3) + 1);
                inc = -inc;
            }
        }

        return arr;
    } */
}