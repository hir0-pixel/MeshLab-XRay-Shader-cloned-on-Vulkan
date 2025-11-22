#include <iostream>
#include <vector>

#include "Config.h"
#include "MeshLoader.h"
#include "MeshUtils.h"
#include "VulkanVertex.h"
#include "VulkanApp.h"

int main() {
    try {
        std::cout << "Mesh path from Config: " << Config::MESH_PATH << "\n";
        MeshData mesh = loadMesh(
            Config::MESH_PATH,
            Config::WRITE_PLY_COPY,
            std::string(Config::PLY_OUT_PATH)
        );

        std::cout << "Final vertex count:   " << mesh.vertices.size()    << "\n";
        std::cout << "Final triangle count: " << mesh.indices.size() / 3 << "\n";

        MeshBounds bBefore = computeBounds(mesh);
        std::cout << "Bounds before normalization:\n";
        std::cout << "  min: " << bBefore.min.x << ", " << bBefore.min.y << ", " << bBefore.min.z << "\n";
        std::cout << "  max: " << bBefore.max.x << ", " << bBefore.max.y << ", " << bBefore.max.z << "\n";
        std::cout << "  center: " << bBefore.center.x << ", " << bBefore.center.y << ", " << bBefore.center.z << "\n";
        std::cout << "  radius: " << bBefore.radius << "\n";

        MeshBounds bAfter;
        normalizeToUnitSphere(mesh, bAfter);

        std::cout << "Bounds after normalization:\n";
        std::cout << "  min: " << bAfter.min.x << ", " << bAfter.min.y << ", " << bAfter.min.z << "\n";
        std::cout << "  max: " << bAfter.max.x << ", " << bAfter.max.y << ", " << bAfter.max.z << "\n";
        std::cout << "  center: " << bAfter.center.x << ", " << bAfter.center.y << ", " << bAfter.center.z << "\n";
        std::cout << "  radius: " << bAfter.radius << "\n";

        std::vector<VulkanVertex> gpuVertices;
        gpuVertices.reserve(mesh.vertices.size());

        for (const auto &v : mesh.vertices) {
            VulkanVertex gv{};
            gv.pos[0]    = v.pos.x;
            gv.pos[1]    = v.pos.y;
            gv.pos[2]    = v.pos.z;
            gv.normal[0] = v.normal.x;
            gv.normal[1] = v.normal.y;
            gv.normal[2] = v.normal.z;
            gpuVertices.push_back(gv);
        }

        std::vector<uint32_t> &gpuIndices = mesh.indices;

        std::cout << "\nConverted to VulkanVertex layout:\n";
        std::cout << "  gpuVertices.size(): " << gpuVertices.size() << "\n";
        std::cout << "  gpuIndices.size():  " << gpuIndices.size()  << " ("
                  << gpuIndices.size() / 3 << " triangles)\n";

        if (!gpuVertices.empty()) {
            const auto &v0 = gpuVertices[0];
            std::cout << "Example vertex[0]: pos = ("
                      << v0.pos[0] << ", " << v0.pos[1] << ", " << v0.pos[2]
                      << "), normal = ("
                      << v0.normal[0] << ", " << v0.normal[1] << ", " << v0.normal[2]
                      << ")\n";
        }

        std::cout << "\nLaunching VulkanApp...\n";

        VulkanApp app(gpuVertices, gpuIndices);
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
