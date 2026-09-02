#pragma once

#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

#include "geometry/mesh.hpp"
#include "vk_buffer.hpp"
#include "vulkan/data/vk_mesh_draw_info.hpp"

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

        // GPU draw info for a previously added mesh submesh, or nullptr if the
        // id or submesh index is out of range.
        const MeshDrawInfo *getDrawInfo(MeshID id, uint32_t subMeshIndex) const;

        // Number of drawable submeshes for a mesh (at least 1).
        uint32_t getSubMeshCount(MeshID id) const;

        // CPU-side material info for one submesh (default SubMesh if missing).
        SubMesh getSubMesh(MeshID id, uint32_t subMeshIndex) const;

        VkBuffer vertexBufferHandle() const { return vertexBuffer_.buffer; }
        VkBuffer indexBufferHandle() const { return indexBuffer_.buffer; }

    private:
        void reallocateAndUpload(VkDeviceSize vertexCapacity, VkDeviceSize indexCapacity);

        void allocate(VkDeviceSize vertexCapacity, VkDeviceSize indexCapacity);

        // Computes per-submesh draw ranges for `mesh` assuming its data starts
        // at vertexBase/indexBase in the unified buffers.
        static std::vector<MeshDrawInfo> buildDrawInfos(const Mesh &mesh,
                                                        uint32_t vertexBase,
                                                        uint32_t indexBase);

        VulkanBuffer vertexBuffer_;
        VulkanBuffer indexBuffer_;

        std::unordered_map<MeshID, Mesh> meshes_; // CPU source data
        std::unordered_map<MeshID, std::vector<MeshDrawInfo>> id2drawInfo_;
        std::vector<MeshID> meshOrder_; // stable upload order

        MeshID nextId_ = 0;
        uint32_t vertexCount_ = 0; // vertices uploaded so far
        uint32_t indexCount_ = 0;  // indices uploaded so far
    };

}