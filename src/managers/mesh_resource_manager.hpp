#pragma once

#include "geometry/mesh.hpp"
#include <unordered_map>

namespace engine
{
    class MeshResourceManager
    {
    public:
        MeshID put(Mesh mesh);
        bool remove(MeshID meshHandle);
        Mesh get(MeshID meshHandle);
        
        static MeshResourceManager &getInstance()
        {
            if (instance == nullptr)
            {
                instance = std::unique_ptr<MeshResourceManager>(std::make_unique<MeshResourceManager>());
            }
            return *instance;
        }

        MeshResourceManager() = default;

    private:

        static std::unique_ptr<MeshResourceManager> instance;
        
        MeshID nextMeshId = 0;
        std::unordered_map<MeshID, Mesh> handle2mesh;
    };
}