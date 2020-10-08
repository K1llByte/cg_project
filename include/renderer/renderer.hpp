#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "renderer/scene.hpp"

namespace cg
{
    namespace renderer
    {
        void init(int* argc, char** argv);

        void run();

        void set_scene(cg::scene*);
    }
}

#endif // RENDERER_HPP