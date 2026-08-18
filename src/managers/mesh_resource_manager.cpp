#include "mesh_resource_manager.hpp"

namespace engine
{

    std::unique_ptr<MeshResourceManager> MeshResourceManager::instance = nullptr;

    MeshID MeshResourceManager::put(Mesh mesh)
    {
        MeshID currentMeshHandle = nextMeshId;
        nextMeshId++;
        handle2mesh.emplace(currentMeshHandle, std::move(mesh));
        return currentMeshHandle;
    }

    bool MeshResourceManager::remove(MeshID meshHandle)
    {
        auto result = handle2mesh.erase(meshHandle);
        if( result > 0 ){

        }
    }

    Mesh MeshResourceManager::get(MeshID meshHandle)
    {
        auto it = handle2mesh.find(meshHandle);
        if(it != handle2mesh.end()){
            return it->second;
        }else{
            throw std::runtime_error("mesh not found!");
        }
    }

}