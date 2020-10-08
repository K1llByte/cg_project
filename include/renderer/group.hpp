#ifndef GROUP_HPP
#define GROUP_HPP

#include "renderer/transformation.hpp"
#include "renderer/model.hpp"

#include <vector>

namespace cg
{
    class group
    {
        private:
            std::vector<transformation>  transformations;
            std::vector<model> models;
            std::vector<group> groups;

        public:
            group() = default;
            ~group() = default;

            void add_transformation(const transformation&);
            void add_model(const model&);
            void add_group(const group&);

            void draw(const float delta_time_sec);
            
            void print() const;
    };
}

#endif // GROUP_HPP