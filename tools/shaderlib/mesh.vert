#version 450

uniform u_vs_pbrParams {
    mat4 modelMatrix;
    mat4 normalMatrix;
    mat4 mvpMatrix;
} pbrParams;

#ifdef HAS_TEXTURERECT
    uniform u_vs_spriteParams {
        vec4 textureRect;
    } spriteParams;
#endif

in vec3 a_position;
out vec3 v_position;

#ifdef HAS_NORMALS
    in vec3 a_normal;
#endif

#ifdef HAS_TANGENTS
    in vec4 a_tangent;
#endif

#ifdef HAS_NORMALS
#ifdef HAS_TANGENTS
    out mat3 v_tbn;
#else
    out vec3 v_normal;
#endif
#endif

#ifdef HAS_UV_SET1
    in vec2 a_texcoord1;
#endif

#ifdef HAS_UV_SET2
    in vec2 a_texcoord2;
#endif

out vec2 v_uv1;
out vec2 v_uv2;

#ifdef HAS_VERTEX_COLOR_VEC3
    in vec3 a_color;
    out vec3 v_color;
#endif

#ifdef HAS_VERTEX_COLOR_VEC4
    in vec4 a_color;
    out vec4 v_color;
#endif

#ifdef USE_SHADOWS
    uniform u_vs_shadows {
        mat4 lightVPMatrix[MAX_SHADOWSMAP];
    };
 
    out vec4 v_lightProjPos[MAX_SHADOWSMAP];
    out float v_clipSpacePosZ;
#endif

#include "includes/skinning.glsl"
#include "includes/morphtarget.glsl"
#ifdef HAS_TERRAIN
    #include "includes/terrain_vs.glsl"
#endif

vec4 getPosition(mat4 boneTransform){
    vec3 pos = a_position;

    pos = getMorphPosition(pos);
    pos = getSkinPosition(pos, boneTransform);
    #ifdef HAS_TERRAIN
        pos = getTerrainPosition(pos, pbrParams.modelMatrix);
    #endif

    return vec4(pos, 1.0);
}

#ifdef HAS_NORMALS
vec3 getNormal(mat4 boneTransform, vec4 position){
    vec3 normal = a_normal;

    normal = getMorphNormal(normal);
    normal = getSkinNormal(normal, boneTransform);
    #ifdef HAS_TERRAIN
        normal = getTerrainNormal(normal, position.xyz);
    #endif

    return normalize(normal);
}
#endif

#ifdef HAS_TANGENTS
vec3 getTangent(mat4 boneTransform){
    vec3 tangent = a_tangent.xyz;

    tangent = getMorphTangent(tangent);
    tangent = getSkinTangent(tangent, boneTransform);

    return normalize(tangent);
}
#endif

void main() {
    mat4 boneTransform = getBoneTransform();

    vec4 pos = getPosition(boneTransform);
    vec4 worldPos = pbrParams.modelMatrix * pos;
    v_position = vec3(worldPos.xyz) / worldPos.w;

    #ifdef HAS_NORMALS
    #ifdef HAS_TANGENTS
        vec3 tangent = getTangent(boneTransform);
        vec3 normalW = normalize(vec3(pbrParams.normalMatrix * vec4(getNormal(boneTransform, pos), 0.0)));
        vec3 tangentW = normalize(vec3(pbrParams.modelMatrix * vec4(tangent, 0.0)));
        vec3 bitangentW = cross(normalW, tangentW) * a_tangent.w;
        v_tbn = mat3(tangentW, bitangentW, normalW);
    #else // !HAS_TANGENTS
        v_normal = normalize(vec3(pbrParams.normalMatrix * vec4(getNormal(boneTransform, pos), 0.0)));
    #endif
    #endif

    v_uv1 = vec2(0.0, 0.0);
    v_uv2 = vec2(0.0, 0.0);

    #ifdef HAS_UV_SET1
        v_uv1 = a_texcoord1;
        #ifdef HAS_TEXTURERECT
            v_uv1 = a_texcoord1 * spriteParams.textureRect.zw + spriteParams.textureRect.xy;
        #endif
    #endif
    #ifdef HAS_TERRAIN
        v_uv1 = getTerrainTiledTexture(pos.xyz);
    #endif

    #ifdef HAS_UV_SET2
        v_uv2 = a_texcoord2;
        #ifdef HAS_TEXTURERECT
            v_uv2 = a_texcoord2 * spriteParams.textureRect.zw + spriteParams.textureRect.xy;
        #endif
    #endif

    #if defined(HAS_VERTEX_COLOR_VEC3) || defined(HAS_VERTEX_COLOR_VEC4)
        v_color = a_color;
    #endif

    #ifdef USE_SHADOWS
    for (int i = 0; i < MAX_SHADOWSMAP; ++i){
        v_lightProjPos[i] = lightVPMatrix[i] * worldPos;
    }
    #endif

    gl_Position = pbrParams.mvpMatrix * pos;

    #ifdef USE_SHADOWS
        v_clipSpacePosZ = gl_Position.z;
    #endif
}