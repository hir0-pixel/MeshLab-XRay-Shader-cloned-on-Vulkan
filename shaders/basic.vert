#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

// MeshLab's varyings (ported):
// N: normal in eye space
// I: position in eye space
// Cs: vertex color
layout(location = 0) out vec3 N;
layout(location = 1) out vec3 I;
layout(location = 2) out vec4 Cs;

// Push constants: we push both MVP and MV
layout(push_constant) uniform PushConsts {
    mat4 mvp;  // = Projection * View * Model
    mat4 mv;   // = View * Model
} pc;

void main() {
    // eye-space position
    vec4 P = pc.mv * vec4(inPos, 1.0);
    I = P.xyz - vec3(0.0);

    // eye-space normal (like gl_NormalMatrix * gl_Normal)
    N = mat3(pc.mv) * inNormal;

    // MeshLab uses gl_Color; we don't have per-vertex colors,
    // so take constant white (you can tint this later in C++).
    Cs = vec4(1.0, 1.0, 1.0, 1.0);

    // clip-space position = Projection * View * Model * pos
    gl_Position = pc.mvp * vec4(inPos, 1.0);
}
