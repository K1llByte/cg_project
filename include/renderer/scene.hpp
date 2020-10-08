#ifndef SCENE_HPP
#define SCENE_HPP

#include "renderer/model.hpp"
#include "renderer/camera.hpp"
#include "renderer/group.hpp"
#include "renderer/light.hpp"

#include <unordered_map>
#include <optional>


namespace cg
{
    class scene
    {
        private:
            std::vector<cg::group> groups;
            std::optional<cg::model> skybox;
            cg::lights lights;
            cg::camera camera;

        public:
            float relative_time_multiplier = 1.0f;
            static bool debug;
            static float frustum[6][4];
            
            scene();
            scene(const std::vector<cg::group>& g, cg::lights& ls, const cg::camera& c = cg::camera(5,5,0));
            scene(const std::vector<cg::group>& g, const cg::model& sb, const cg::camera& c = cg::camera(5,5,0));
            scene(const std::vector<cg::group>& g, const cg::model& sb, cg::lights& ls, const cg::camera& c = cg::camera(5,5,0));
            ~scene() = default;

            void set_camera(const cg::camera& c);

            void compose();

            std::vector<cg::group>& get_groups();
            cg::camera& get_camera();
    };

    void extract_frustum(void);
    bool boundingbox_in_frustum(const std::array<cg::vec4,8>& pointlist);
}

#endif // SCENE_HPP