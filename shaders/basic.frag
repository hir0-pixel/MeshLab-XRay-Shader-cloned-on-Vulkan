#version 450

layout(location = 0) in vec3 N;
layout(location = 1) in vec3 I;
layout(location = 2) in vec4 Cs;

layout(location = 0) out vec4 outColor;

// MeshLab defaults (from xray.gdp)
const float edgefalloff = 1.0;
const float intensity   = 0.5;
const float ambient     = 0.01;


void main() {
    float opac = dot(normalize(-N), normalize(-I));
    opac = abs(opac);
    opac = ambient + intensity * (1.0 - pow(opac, edgefalloff));

    opac *= 1.1f;
    opac = clamp(opac, 0.0, 1.0);

    vec3 tint       = vec3(0.80, 0.90, 1.00);
    vec3 xrayColor  = Cs.rgb * tint * opac;

    outColor = vec4(xrayColor, opac);
}
