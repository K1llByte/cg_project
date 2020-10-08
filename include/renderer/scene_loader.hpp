#ifndef SCENE_LOADER_HPP
#define SCENE_LOADER_HPP

#include "renderer/scene.hpp"

namespace cg
{
    /* std::optional<scene> xml_to_scene(const std::filesystem::path& file_name); */

    std::optional<scene> xml_to_scene(const std::filesystem::path& file_name);
}

#endif // SCENE_LOADER_HPP