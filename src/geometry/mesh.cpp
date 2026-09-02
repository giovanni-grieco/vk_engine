#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <unordered_map>

#include "mesh.hpp"
#include "vertex.hpp"

namespace engine
{
    Mesh createMeshFromFile(std::string filepath)
    {
        Mesh mesh{};

        // Resolve the OBJ's own directory so the .mtl and its textures are
        // searched next to the model (not in the app's working directory).
        std::string objDir = filepath.substr(0, filepath.find_last_of("/\\") + 1);

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str(),
                              objDir.c_str(), true /* triangulate */, false /* default_vcols_fallback */))
        {
            throw std::runtime_error(err);
        }
        if(!warn.empty()){
            std::cout << "Warning while loading \""+filepath+"\": "+warn+"\n";
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        // Group indices per material so each submesh ends up as one contiguous
        // range, no matter how faces are interleaved in the file.
        std::unordered_map<int, std::vector<uint32_t>> materialIndices;
        std::vector<int> materialOrder; // first-appearance order

        auto indexForVertex = [&](const tinyobj::index_t &index) -> uint32_t {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.texcoord_index >= 0 &&
                static_cast<size_t>(2 * index.texcoord_index + 1) < attrib.texcoords.size())
            {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (index.normal_index >= 0 &&
                static_cast<size_t>(3 * index.normal_index + 2) < attrib.normals.size())
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0){
                uniqueVertices[vertex] = static_cast<uint32_t>(mesh.vertices.size());
                mesh.vertices.push_back(vertex);
            }
            return uniqueVertices[vertex];
        };

        for(const auto& shape: shapes){
            const auto &smesh = shape.mesh;
            size_t faceCount = smesh.num_face_vertices.size();
            size_t indexCursor = 0;

            for (size_t f = 0; f < faceCount; ++f)
            {
                const int materialId =
                    (f < smesh.material_ids.size()) ? smesh.material_ids[f] : -1;

                auto it = materialIndices.find(materialId);
                if (it == materialIndices.end())
                {
                    materialOrder.push_back(materialId);
                    materialIndices.emplace(materialId, std::vector<uint32_t>{});
                    it = materialIndices.find(materialId);
                }

                const uint8_t vertsPerFace = smesh.num_face_vertices[f];
                for (uint8_t v = 0; v < vertsPerFace; ++v)
                {
                    const tinyobj::index_t &index = smesh.indices[indexCursor + v];
                    it->second.push_back(indexForVertex(index));
                }
                indexCursor += vertsPerFace;
            }
        }

        // Concatenate per-material index lists and record each submesh range.
        for (int materialId : materialOrder)
        {
            SubMesh sub{};
            sub.firstIndex = static_cast<uint32_t>(mesh.indices.size());

            const std::vector<uint32_t> &indices = materialIndices[materialId];
            mesh.indices.insert(mesh.indices.end(), indices.begin(), indices.end());
            sub.indexCount = static_cast<uint32_t>(indices.size());

            if (materialId >= 0 && materialId < static_cast<int>(materials.size()))
            {
                const tinyobj::material_t &mat = materials[materialId];
                sub.materialName = mat.name;
                sub.diffuseColor = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]};

                if (!mat.diffuse_texname.empty())
                {
                    const std::string &tex = mat.diffuse_texname;
                    const bool absolute = (!tex.empty() && tex[0] == '/') ||
                                          (tex.size() > 1 && tex[1] == ':');
                    sub.diffuseTexturePath = absolute ? tex : (objDir + tex);
                }
            }

            mesh.subMeshes.push_back(sub);
        }

        return mesh;
    }

    void Mesh::dump(){
        std::cout << "Mesh dump:\n\t"<<"vertices:\t"<<vertices.size()<<"\n\t"<<"indices:\t"<<indices.size()<<"\n"
                  << "\t" << "submeshes:\t" << (subMeshes.empty() ? 1 : subMeshes.size()) << "\n";
        for (const auto &sub : subMeshes)
        {
            std::cout << "\t\t- \"" << sub.materialName << "\" indices: " << sub.indexCount
                      << " texture: \"" << sub.diffuseTexturePath << "\"\n";
        }
    }
}