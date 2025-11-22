#pragma once

namespace Config
{
    inline constexpr const char* MESH_PATH =
        "D:/Shader Optimization/assets/meshes/Armadillo.ply";

    inline constexpr bool WRITE_PLY_COPY = false;
    inline constexpr const char* PLY_OUT_PATH = "";
    // --------------------------------
    // Shader controls
    // --------------------------------
    inline bool ENABLE_BACKFACE_CULLING = false;  // off by default
}
