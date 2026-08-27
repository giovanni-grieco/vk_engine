#include "vk_mesh_buffer_manager.hpp"

#include <algorithm>

namespace engine
{
    VulkanBufferManager::VulkanBufferManager(VulkanDevice &device, VulkanCommandPool &commandPool, VulkanQueues &queues)
        : vertexBuffer_(device, commandPool, queues),
          indexBuffer_(device, commandPool, queues)
    {
        allocate(2048, 256);
    }

    MeshID VulkanBufferManager::addMesh(const Mesh &mesh)
    {
        const MeshID id = nextId_++;
        meshes_.emplace(id, mesh);
        meshOrder_.push_back(id);

        // Append at the end of the currently-written region. Note: this stays
        // ahead of the live data when holes exist (removed meshes), which is
        // safe — holes are simply left unused until the next reallocation.
        const uint32_t vertexBase = vertexCount_;
        const uint32_t indexBase = indexCount_;

        const uint32_t newVertexCount = vertexCount_ + static_cast<uint32_t>(mesh.vertices.size());
        const uint32_t newIndexCount = indexCount_ + static_cast<uint32_t>(mesh.indices.size());

        const VkDeviceSize neededVertexBytes = static_cast<VkDeviceSize>(newVertexCount) * sizeof(Vertex);
        const VkDeviceSize neededIndexBytes = static_cast<VkDeviceSize>(newIndexCount) * sizeof(uint32_t);

        if (neededVertexBytes > vertexBuffer_.size || neededIndexBytes > indexBuffer_.size)
        {
            // Grow (double the previous capacity) and re-upload everything compactly.
            const VkDeviceSize vertexCapacity = std::max(neededVertexBytes, static_cast<VkDeviceSize>(vertexBuffer_.size * 2));
            const VkDeviceSize indexCapacity = std::max(neededIndexBytes, static_cast<VkDeviceSize>(indexBuffer_.size * 2));
            reallocateAndUpload(vertexCapacity, indexCapacity);
            std::cout << "telescope increase of Vulkan buffer:\n\t" << "vertexCapacity bytes: " << vertexCapacity << "\n\t" << "indexCapacity bytes: " << indexCapacity << "\n";
        }
        else
        {
            // Fits in the existing buffers: just append this mesh.
            MeshDrawInfo info;
            info.vertexOffset = static_cast<int32_t>(vertexBase);
            info.firstIndex = indexBase;
            info.indexCount = static_cast<uint32_t>(mesh.indices.size());
            id2drawInfo_[id] = info;

            vertexBuffer_.upload(mesh.vertices.data(),
                                 mesh.vertices.size() * sizeof(Vertex),
                                 static_cast<VkDeviceSize>(vertexBase) * sizeof(Vertex));
            indexBuffer_.upload(mesh.indices.data(),
                                mesh.indices.size() * sizeof(uint32_t),
                                static_cast<VkDeviceSize>(indexBase) * sizeof(uint32_t));

            vertexCount_ = newVertexCount;
            indexCount_ = newIndexCount;
        }

        return id;
    }

    void VulkanBufferManager::removeMesh(MeshID id)
    {
        meshes_.erase(id);
        id2drawInfo_.erase(id);
        meshOrder_.erase(std::remove(meshOrder_.begin(), meshOrder_.end(), id), meshOrder_.end());

        // vertexCount_/indexCount_ are intentionally left untouched: the removed
        // data stays in the buffers as an unreachable hole until compact() runs.
    }

    void VulkanBufferManager::compact()
    {
        // The buffers may still be referenced by in-flight frames, so wait for
        // everything to finish before freeing/recreating them.
        vkDeviceWaitIdle(vertexBuffer_.device);

        VkDeviceSize vertexBytes = 0;
        VkDeviceSize indexBytes = 0;
        for (MeshID id : meshOrder_)
        {
            const Mesh &mesh = meshes_.at(id);
            vertexBytes += static_cast<VkDeviceSize>(mesh.vertices.size()) * sizeof(Vertex);
            indexBytes += static_cast<VkDeviceSize>(mesh.indices.size()) * sizeof(uint32_t);
        }

        if (vertexBytes == 0 && indexBytes == 0)
        {
            vertexBuffer_.destroy();
            indexBuffer_.destroy();
            vertexCount_ = 0;
            indexCount_ = 0;
            return;
        }

        // Rebuild exactly to the live data size (shrinks if holes were left).
        reallocateAndUpload(vertexBytes, indexBytes);
    }

    const MeshDrawInfo *VulkanBufferManager::getDrawInfo(MeshID id) const
    {
        const auto it = id2drawInfo_.find(id);
        return it == id2drawInfo_.end() ? nullptr : &it->second;
    }

    void VulkanBufferManager::allocate(VkDeviceSize vertexCapacity, VkDeviceSize indexCapacity)
    {
        vertexBuffer_.create(BufferType::VERTEX, vertexCapacity);
        indexBuffer_.create(BufferType::INDEX, indexCapacity);
    }

    void VulkanBufferManager::reallocateAndUpload(VkDeviceSize vertexCapacity, VkDeviceSize indexCapacity)
    {
        vertexBuffer_.destroy();
        indexBuffer_.destroy();

        vertexBuffer_.create(BufferType::VERTEX, vertexCapacity);
        indexBuffer_.create(BufferType::INDEX, indexCapacity);

        uint32_t vertexBase = 0;
        uint32_t indexBase = 0;

        for (MeshID id : meshOrder_)
        {
            const Mesh &mesh = meshes_.at(id);

            MeshDrawInfo info;
            info.vertexOffset = static_cast<int32_t>(vertexBase);
            info.firstIndex = indexBase;
            info.indexCount = static_cast<uint32_t>(mesh.indices.size());
            id2drawInfo_[id] = info;

            vertexBuffer_.upload(mesh.vertices.data(),
                                 mesh.vertices.size() * sizeof(Vertex),
                                 static_cast<VkDeviceSize>(vertexBase) * sizeof(Vertex));
            indexBuffer_.upload(mesh.indices.data(),
                                mesh.indices.size() * sizeof(uint32_t),
                                static_cast<VkDeviceSize>(indexBase) * sizeof(uint32_t));

            vertexBase += static_cast<uint32_t>(mesh.vertices.size());
            indexBase += static_cast<uint32_t>(mesh.indices.size());
        }

        // The buffers now hold exactly the live meshes, compactly.
        vertexCount_ = vertexBase;
        indexCount_ = indexBase;
    }
}
