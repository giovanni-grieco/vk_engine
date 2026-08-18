#pragma once

#include "geometry/mesh.hpp"

#include <vector>

namespace engine{
    struct MeshComponent{
        
        std::vector<Mesh> mesh;

        MeshID meshHandle = -1;

    };
}