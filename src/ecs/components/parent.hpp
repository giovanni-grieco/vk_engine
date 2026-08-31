#pragma once

#include "ecs/entity/entity.hpp"

namespace engine{
    
    struct ParentComponent{
        Entity parent = NULL_ENTITY;
    };
}