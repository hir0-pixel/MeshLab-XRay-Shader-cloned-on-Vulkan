#pragma once

#include "MeshLoader.h"

#include <glm/vec3.hpp>
#include <limits>
#include <algorithm>
#include <cmath>

struct MeshBounds {
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 center;
    float     radius;
};

inline MeshBounds computeBounds(const MeshData &mesh) {
    MeshBounds b{};
    b.min = glm::vec3(
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    );
    b.max = glm::vec3(
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest()
    );

    for (const auto &v : mesh.vertices) {
        b.min.x = std::min(b.min.x, v.pos.x);
        b.min.y = std::min(b.min.y, v.pos.y);
        b.min.z = std::min(b.min.z, v.pos.z);

        b.max.x = std::max(b.max.x, v.pos.x);
        b.max.y = std::max(b.max.y, v.pos.y);
        b.max.z = std::max(b.max.z, v.pos.z);
    }

    b.center = 0.5f * (b.min + b.max);

    float maxR2 = 0.0f;
    for (const auto &v : mesh.vertices) {
        glm::vec3 d = v.pos - b.center;
        float r2 = d.x * d.x + d.y * d.y + d.z * d.z;
        if (r2 > maxR2) maxR2 = r2;
    }
    b.radius = maxR2 > 0.0f ? std::sqrt(maxR2) : 0.0f;
    return b;
}

inline void normalizeToUnitSphere(MeshData &mesh, MeshBounds &boundsOut) {
    boundsOut = computeBounds(mesh);
    glm::vec3 c = boundsOut.center;
    float r = boundsOut.radius;

    float scale = (r > 0.0f) ? (1.0f / r) : 1.0f;

    for (auto &v : mesh.vertices) {
        v.pos = (v.pos - c) * scale;
    }

    boundsOut = computeBounds(mesh);
}
