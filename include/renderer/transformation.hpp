#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <variant>
#include <vector>
#include <array>
#include "common/utils.hpp"
#include "renderer/vbo.hpp"

namespace cg
{    
    class translation 
    {
        private:
            const float x, y, z;

        public:
            translation(float t_x, float t_y, float t_z);

            void apply(const float delta_time_sec);
    };

    class rotation
    {
        private:
            const float angle, x, y, z;

        public:
            rotation(float t_angle, float t_x, float t_y, float t_z);

            void apply(const float delta_time_sec);
    };

    class scale 
    {
        private:
            const float x, y, z;

        public:
            scale(float t_x, float t_y, float t_z);

            void apply(const float delta_time_sec);
    };

    class translation_timed
    {
        private:
            const std::vector<vec3> positions;
            const float time_sec;
            const bool closed;
            float pos_time_sec;

            cg::vbo vbo;

        public:
            translation_timed(const float t_time_sec, const std::vector<vec3>& t_positions, const bool t_closed = true);

            void apply(const float delta_time_sec);
    };

    class rotation_timed
    {
        private:
            const float x, y, z;
            const float time_sec;
            float pos_time_sec;

        public:
            rotation_timed(const float t_time_sec, const float x, const float y, const float z);

            void apply(const float delta_time_sec);
    };

    class transformation
    {
        private:
            std::variant<translation,rotation,scale,translation_timed,rotation_timed> value;

        public:
            transformation() = default;
            transformation(const translation&);
            transformation(const rotation&);
            transformation(const scale&);
            transformation(const translation_timed&);
            transformation(const rotation_timed&);
            //transformation(const transformation&) = default;
            //transformation(const transformation&&) = default;
            
            void apply(const float delta_time_sec);
    };
}

#endif // TRANSFORMATION_HPP