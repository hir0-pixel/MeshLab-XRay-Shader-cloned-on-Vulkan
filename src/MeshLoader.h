#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

struct MeshData {
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
};

MeshData loadMesh(
    const std::string& path,
    bool writePlyCopy = false,
    const std::string& plyOutPath = ""
);

void writeMeshAsPly(
    const MeshData& mesh,
    const std::string& path
);
