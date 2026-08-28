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

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            throw std::runtime_error(err);
        }
        if(!warn.empty()){
            std::cout << "Warning while loading \""+filepath+"\": "+warn+"\n";
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for(const auto& shape: shapes){
            for(const auto& index : shape.mesh.indices){
                Vertex vertex{};
                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0){
                    uniqueVertices[vertex] = static_cast<uint32_t>(mesh.vertices.size());
                    mesh.vertices.push_back(vertex);
                }

                mesh.indices.push_back(uniqueVertices[vertex]);
            }
        }



        return mesh;
    }

    void Mesh::dump(){
        std::cout << "Mesh dump:\n\t"<<"vertices:\t"<<vertices.size()<<"\n\t"<<"indices:\t"<<indices.size()<<"\n";
    }
}