#pragma once

#include <cstdint>
#include <vector>

#include <string>

#include "vertex.hpp"

namespace engine{

    typedef int32_t MeshID;

    // A contiguous drawable range of a mesh, produced by one material group
    // of an .obj file. `firstIndex`/`indexCount` index into Mesh::indices.
    struct SubMesh{
        uint32_t firstIndex = 0;
        uint32_t indexCount = 0;
        std::string materialName;
        std::string diffuseTexturePath; // resolved at load time; empty if none
        glm::vec3 diffuseColor{1.0f, 1.0f, 1.0f};
    };

    struct Mesh{
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        // One entry per material group. Empty means "draw the whole mesh as a
        // single submesh" (used by procedurally built meshes).
        std::vector<SubMesh> subMeshes;

        void dump();
    };

    Mesh createMeshFromFile(std::string filepath);
}