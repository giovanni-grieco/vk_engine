#pragma once

#include <stdint.h>
#include <vector>

#include <glm/glm.hpp>
#include <string>

#include "vertex.hpp"

namespace engine{

    typedef int32_t MeshID;

    struct Mesh{
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    Mesh createMeshFromFile(std::string filepath);
}