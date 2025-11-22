#include "MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>

// ----------------------------------------
// helpers
// ----------------------------------------

static std::string toLowerExt(const std::string& path)
{
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos)
        return "";

    std::string ext = path.substr(dotPos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return ext;
}

// ----------------------------------------
// main load function
// ----------------------------------------

MeshData loadMesh(
    const std::string& path,
    bool writePlyCopy,
    const std::string& plyOutPath
) {
    std::string ext = toLowerExt(path);
    std::cout << "Loading mesh: " << path << "\n";
    std::cout << "Extension: " << ext << "\n";

    Assimp::Importer importer;
    unsigned int flags = 0;

    if (ext == "ply") {
        flags = aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_GenNormals;
        std::cout << "Using PLY import flags\n";
    } else if (ext == "stl") {
        flags = aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_GenNormals;
        std::cout << "Using STL import flags\n";
    } else if (ext == "obj") {
        flags = aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_GenSmoothNormals;
        std::cout << "Using OBJ import flags\n";
    } else {
        flags = aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_GenNormals;
        std::cout << "Unknown extension, using default flags\n";
    }

    std::cout << "Calling Assimp ReadFile...\n";
    const aiScene* scene = importer.ReadFile(path, flags);
    std::cout << "ReadFile returned\n";

    if (!scene) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << "\n";
        throw std::runtime_error("Failed to load mesh");
    }
    if (!scene->HasMeshes()) {
        throw std::runtime_error("Scene has no meshes");
    }

    const aiMesh* mesh = scene->mMeshes[0];

    MeshData data;
    data.vertices.reserve(mesh->mNumVertices);
    data.indices.reserve(mesh->mNumFaces * 3);

    std::cout << "Vertices in mesh: " << mesh->mNumVertices << "\n";
    std::cout << "Faces in mesh:    " << mesh->mNumFaces << "\n";

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        aiVector3D p = mesh->mVertices[i];
        aiVector3D n = mesh->HasNormals() ? mesh->mNormals[i] : aiVector3D(0, 0, 1);

        Vertex v;
        v.pos    = { p.x, p.y, p.z };
        v.normal = { n.x, n.y, n.z };
        data.vertices.push_back(v);
    }

    for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
        const aiFace& face = mesh->mFaces[f];
        if (face.mNumIndices != 3) continue;
        data.indices.push_back(face.mIndices[0]);
        data.indices.push_back(face.mIndices[1]);
        data.indices.push_back(face.mIndices[2]);
    }

    std::cout << "Loaded vertices: "  << data.vertices.size()    << "\n";
    std::cout << "Loaded triangles: " << data.indices.size() / 3 << "\n";

    if (writePlyCopy) {
        std::string out = plyOutPath;
        if (out.empty()) {
            size_t dotPos = path.find_last_of('.');
            if (dotPos == std::string::npos)
                out = path + ".out.ply";
            else
                out = path.substr(0, dotPos) + ".out.ply";
        }
        std::cout << "Writing PLY copy to: " << out << "\n";
        writeMeshAsPly(data, out);
    }

    return data;
}

// ----------------------------------------
// PLY writer
// ----------------------------------------

void writeMeshAsPly(
    const MeshData& mesh,
    const std::string& path
) {
    std::ofstream ofs(path, std::ios::out | std::ios::trunc);
    if (!ofs) {
        throw std::runtime_error("Failed to open PLY file for writing: " + path);
    }

    const size_t vertexCount   = mesh.vertices.size();
    const size_t triangleCount = mesh.indices.size() / 3;

    ofs << "ply\n";
    ofs << "format ascii 1.0\n";
    ofs << "element vertex " << vertexCount << "\n";
    ofs << "property float x\n";
    ofs << "property float y\n";
    ofs << "property float z\n";
    ofs << "property float nx\n";
    ofs << "property float ny\n";
    ofs << "property float nz\n";
    ofs << "element face " << triangleCount << "\n";
    ofs << "property list uchar int vertex_indices\n";
    ofs << "end_header\n";

    for (const auto& v : mesh.vertices) {
        ofs << v.pos.x    << " "
            << v.pos.y    << " "
            << v.pos.z    << " "
            << v.normal.x << " "
            << v.normal.y << " "
            << v.normal.z << "\n";
    }

    for (size_t i = 0; i < triangleCount; ++i) {
        uint32_t i0 = mesh.indices[3 * i + 0];
        uint32_t i1 = mesh.indices[3 * i + 1];
        uint32_t i2 = mesh.indices[3 * i + 2];
        ofs << "3 " << i0 << " " << i1 << " " << i2 << "\n";
    }

    std::cout << "PLY written, vertices: " << vertexCount
              << " triangles: " << triangleCount << "\n";
}
