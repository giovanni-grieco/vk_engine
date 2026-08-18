#pragma once

#include <stdint.h>
#include <vector>

#include "vertex.hpp"

namespace engine{

    typedef uint32_t MeshID;

    struct Mesh{
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
    };
}