uniform texture2D u_heightMap;
uniform sampler u_heightMap_smp;

uniform u_vs_terrainParams {
    vec3 eyePos;
    float size;
    float maxHeight;
    float resolution;
    float textureBaseTiles; //int
    float textureDetailTiles; //int
} terrain;

uniform u_vs_terrainNodeParams {
    vec2 pos;
    float size;
    float range;
    float resolution; //int
} terrainNode;

out vec2 v_terrainTextureCoords;
out vec2 v_terrainTextureDetailTiled;


float morphFactor;


vec2 morphVertex(vec2 gridPos, vec2 worldPos, float morph) {
    vec2 gridDim = vec2(terrainNode.resolution, terrainNode.resolution);

    vec2 fracPart = fract(gridPos * gridDim.xy * 0.5) * 2.0 / gridDim.xy;
    return worldPos - fracPart * terrainNode.size * morph;
}

float getHeight(vec3 position) {
    return texture(sampler2D(u_heightMap, u_heightMap_smp), (position.xz + (terrain.size/2.0)) / terrain.size).r * terrain.maxHeight;
}

// must be called BEFORE getTerrainNormal because morphValue
vec3 getTerrainPosition(vec3 pos, mat4 modelMatrix){
    pos = terrainNode.size * pos;
    pos = pos + vec3(terrainNode.pos[0], 0.0, terrainNode.pos[1]);

    pos = vec3(pos.x, getHeight(pos), pos.z);

    float morphStart = 0.0;
    float morphEnd = 0.4;

    float dist = distance(terrain.eyePos, vec3(modelMatrix * vec4(pos, 1.0)));

    float nextlevel_thresh = ((terrainNode.range - dist) / terrainNode.size * terrainNode.resolution / float(terrain.resolution));
    morphFactor = 1.0 - smoothstep(morphStart, morphEnd, nextlevel_thresh);

    pos.xz = morphVertex(a_position.xz, pos.xz, morphFactor);

    pos = vec3(pos.x, getHeight(pos), pos.z);

    return pos;
}

vec3 getTerrainNormal(vec3 normal, vec3 position){
    #ifdef HAS_NORMALS
        float delta = (morphFactor + 1.0) * terrainNode.size / float(terrainNode.resolution);

        vec3 dA = delta * normalize(cross(normal.yzx, normal));
        vec3 dB = delta * normalize(cross(dA, normal));
        vec3 p = position;
        vec3 pA = position + dA;
        vec3 pB = position + dB;

        float h = getHeight(position);
        float hA = getHeight(pA);
        float hB = getHeight(pB);

        p += normal * h;
        pA += normal * hA;
        pB += normal * hB;

        normal = normalize(cross(pB - p, pA - p));
    #endif

    return normal;
}

vec2 getTerrainTiledTexture(vec3 position){
    v_terrainTextureCoords = (position.xz + (terrain.size/2.0)) / terrain.size;
    v_terrainTextureDetailTiled = v_terrainTextureCoords * float(terrain.textureDetailTiles);
    return v_terrainTextureCoords * float(terrain.textureBaseTiles);
}