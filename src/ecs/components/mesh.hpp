#pragma once

#include "geometry/mesh.hpp"

#include <vector>

namespace engine
{
    struct MeshComponent
    {
        Mesh mesh;
        MeshID meshHandle = -1;
    };
}