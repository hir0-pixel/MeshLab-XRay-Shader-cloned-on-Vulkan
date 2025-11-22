# Vulkan X-Ray Shader (MeshLab XRay Implementation)

This project implements the **MeshLab XRay shader technique** inside a real-time Vulkan renderer.

## 🎯 Overview

This renderer loads a high-resolution mesh (Armadillo, Lucy, etc.) using **Assimp**, normalizes it to unit scale, converts it to a Vulkan GPU layout, and renders it using the original XRay shading model extracted from MeshLab (`xray.vert`, `xray.frag`, `xray.gdp`).

The shader reproduces:
- curved transparency falloff
- soft ambient opacity contribution
- view-dependent highlight fading
- edge-based opacity variation

It is visually equivalent to MeshLab's XRay mode.

---

## 🧱 Features

✔ Vulkan rendering  
✔ MeshLab XRay shading logic  
✔ Alpha-blending pipeline  
✔ Orbit camera  
✔ Scroll-wheel zoom  
✔ Assimp mesh import (PLY, STL, OBJ)  
✔ Automatic normalization  
✔ Configurable mesh path through config  
✔ RTX-grade performance

---

## 🔧 How to Use

### 1. Set your mesh path

Edit:
src/config.h

Change:

```cpp
static constexpr const char* MESH_PATH = 
    "D:/Shader Optimization/assets/meshes/Armadillo.ply";
```
2. Run

The app loads and displays the mesh with the XRay shader.

🖱 Controls
Feature	Control
Orbit camera	Left mouse drag
Zoom	Mouse scroll

## 📂 Mesh requirements

Recommended formats:

- **PLY(most recommended)**
- **OBJ**
- **STL**


## High-Poly Tested Meshes

This X-Ray renderer has been tested on multi-million polygon meshes, including:

- **Stanford Armadillo**
- **Stanford Lucy**

Both run smoothly on modern GPUs (e.g., RTX 30-series) with the current Vulkan pipeline.

---

## ⚙ Build Dependencies

This project uses:

- **Vulkan SDK**
- **GLFW** – windowing + input
- **Assimp** – mesh loading (PLY / OBJ / STL, etc.)
- **GLM** – math library (matrices, vectors)

All of these are set up via **vcpkg** in the current project configuration.

---

## 📝 Project Goals

The main goal of this project is to **reproduce MeshLab’s XRay shader physically and visually** inside a modern Vulkan renderer that:

- Uses GPU vertex/index buffers for efficient rendering
- Handles **large, high-poly meshes** robustly
- Implements proper **alpha blending** for X-ray translucency
- Stays **modular and extendable** for future research and optimization work

---

## 📌 Current Status

- ✅ **Core XRay shader** implemented (MeshLab-inspired logic)
- ✅ Successfully tested on **multi-million polygon meshes** (Armadillo, Lucy)
- ✅ Stable real-time rendering with orbit camera + zoom
- 🚧 Future work: shader optimization, performance profiling, and advanced visualization tweaks
