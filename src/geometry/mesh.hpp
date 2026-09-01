#pragma once

#include <cstdint>
#include <vector>

#include <string>

#include "vertex.hpp"

namespace engine{

    typedef int32_t MeshID;

    struct Mesh{
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        void dump();
    };

    Mesh createMeshFromFile(std::string filepath);
}