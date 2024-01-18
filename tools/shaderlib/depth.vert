#version 450

#define DEPTH_SHADER

uniform u_vs_depthParams {
    mat4 modelMatrix;
    mat4 lightVPMatrix;
} depthParams;

in vec3 a_position;
out vec2 v_projZW;

#include "includes/skinning.glsl"
#include "includes/morphtarget.glsl"
#ifdef HAS_TERRAIN
    #include "includes/terrain_vs.glsl"
#endif

void main() {

    mat4 lightMVPMatrix = depthParams.lightVPMatrix * depthParams.modelMatrix;

    vec3 pos = a_position;

    pos = getMorphPosition(pos);
    pos = getSkinPosition(pos, getBoneTransform());
    #ifdef HAS_TERRAIN
        pos = getTerrainPosition(pos, depthParams.modelMatrix);
    #endif

    gl_Position = lightMVPMatrix * vec4(pos, 1.0);
    v_projZW = gl_Position.zw;
    #ifndef IS_GLSL
        gl_Position.y = -gl_Position.y;
    #endif
}