#pragma once

#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

#include "geometry/mesh.hpp"
#include "vk_buffer.hpp"

namespace engine
{

    // Owns the unified vertex + index buffers and maps MeshID -> MeshDrawInfo.
    class VulkanBufferManager
    {
    public:
        VulkanBufferManager(VulkanDevice &device, VulkanCommandPool &commandPool, VulkanQueues &queues);

        VulkanBufferManager(const VulkanBufferManager &) = delete;
        VulkanBufferManager &operator=(const VulkanBufferManager &) = delete;

        // Uploads a mesh and returns its handle. Handles are assigned sequentially.
        MeshID addMesh(const Mesh &mesh);

        // Marks a mesh as deleted. Its GPU data becomes an unreachable hole
        // until compact() is called. Safe to call any time.
        void removeMesh(MeshID id);

        // Rebuilds the buffers with only the live meshes, shrinking the
        // allocation if holes were left behind. Stalls the device, so call
        // it when no frames are in flight (e.g. scene load/unload).
        void compact();

        // GPU draw info for a previously added mesh, or nullptr if the id is unknown.
        const MeshDrawInfo *getDrawInfo(MeshID id) const;

        VkBuffer vertexBufferHandle() const { return vertexBuffer_.buffer; }
        VkBuffer indexBufferHandle() const { return indexBuffer_.buffer; }

    private:
        void reallocateAndUpload(VkDeviceSize vertexCapacity, VkDeviceSize indexCapacity);

        void allocate(VkDeviceSize vertexCapacity, VkDeviceSize indexCapacity);

        VulkanBuffer vertexBuffer_;
        VulkanBuffer indexBuffer_;

        std::unordered_map<MeshID, Mesh> meshes_; // CPU source data
        std::unordered_map<MeshID, MeshDrawInfo> id2drawInfo_;
        std::vector<MeshID> meshOrder_; // stable upload order

        MeshID nextId_ = 0;
        uint32_t vertexCount_ = 0; // vertices uploaded so far
        uint32_t indexCount_ = 0;  // indices uploaded so far
    };

}