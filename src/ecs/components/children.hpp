#pragma once

#include "ecs/entity/entity.hpp"

#include <vector>


namespace engine{
    struct ChildrenComponent{
        std::vector<Entity> children;
    };
}