//
// (c) 2022 Eduardo Doria.
//

#include "MeshSystem.h"

#include "Scene.h"
#include "Engine.h"
#include "buffer/InterleavedBuffer.h"
#include "io/FileData.h"
#include "io/Data.h"

#include <sstream>
#include "tiny_obj_loader.h"
#include "tiny_gltf.h"

using namespace Supernova;


MeshSystem::MeshSystem(Scene* scene): SubSystem(scene){
    signature.set(scene->getComponentType<MeshComponent>());
}

MeshSystem::~MeshSystem(){

}

void MeshSystem::createSprite(SpriteComponent& sprite, MeshComponent& mesh, CameraComponent& camera){
    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLES;
    mesh.submeshes[0].hasTextureRect = true;
    mesh.numSubmeshes = 1;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    mesh.buffer.setUsage(BufferUsage::DYNAMIC);

    mesh.indices.clear();

    Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);

    Texture& mainTexture = mesh.submeshes[0].material.baseColorTexture;

    mainTexture.load();
    unsigned int texWidth = mainTexture.getWidth();
    unsigned int texHeight = mainTexture.getHeight();

    if (sprite.width == 0 && sprite.height == 0){
        sprite.width = texWidth;
        sprite.height = texHeight;
    }

    Vector2 pivotPos = Vector2(0, 0);

    if (sprite.pivotPreset == PivotPreset::BOTTOM_RIGHT){
        pivotPos.x = sprite.width;
    }else if (sprite.pivotPreset == PivotPreset::TOP_RIGHT){
        pivotPos.x = sprite.width;
        pivotPos.y = sprite.height;
    }else if (sprite.pivotPreset == PivotPreset::TOP_LEFT){
        pivotPos.y = sprite.height;
    }else if (sprite.pivotPreset == PivotPreset::TOP_CENTER){
        pivotPos.x = sprite.width / 2.0;
        pivotPos.y = sprite.height;
    }else if (sprite.pivotPreset == PivotPreset::BOTTOM_CENTER){
        pivotPos.x = sprite.width / 2.0;
    }else if (sprite.pivotPreset == PivotPreset::RIGHT_CENTER){
        pivotPos.x = sprite.width;
        pivotPos.y = sprite.height / 2.0;
    }else if (sprite.pivotPreset == PivotPreset::LEFT_CENTER){
        pivotPos.y = sprite.height / 2.0;
    }else if (sprite.pivotPreset == PivotPreset::CENTER){
        pivotPos.x = sprite.width / 2.0;
        pivotPos.y = sprite.height / 2.0;
    }

    if (camera.type == CameraType::CAMERA_2D){
        pivotPos.y = sprite.height - pivotPos.y;
    }

    mesh.buffer.addVector3(attVertex, Vector3(-pivotPos.x, -pivotPos.y, 0));
    mesh.buffer.addVector3(attVertex, Vector3(sprite.width-pivotPos.x, -pivotPos.y, 0));
    mesh.buffer.addVector3(attVertex, Vector3(sprite.width-pivotPos.x,  sprite.height-pivotPos.y, 0));
    mesh.buffer.addVector3(attVertex, Vector3(-pivotPos.x,  sprite.height-pivotPos.y, 0));

    Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);

    float texCutRatioW = 0;
    float texCutRatioH = 0;
    if (texWidth != 0 && texHeight != 0){
        texCutRatioW = 1.0 / texWidth * sprite.textureCutFactor;
        texCutRatioH = 1.0 / texHeight * sprite.textureCutFactor;
    }

    if (!sprite.flipY){ 
        mesh.buffer.addVector2(attTexcoord, Vector2(texCutRatioW, texCutRatioH));
        mesh.buffer.addVector2(attTexcoord, Vector2(1.0-texCutRatioW, texCutRatioH));
        mesh.buffer.addVector2(attTexcoord, Vector2(1.0-texCutRatioW, 1.0-texCutRatioH));
        mesh.buffer.addVector2(attTexcoord, Vector2(texCutRatioW, 1.0-texCutRatioH));
    }else{
        mesh.buffer.addVector2(attTexcoord, Vector2(texCutRatioW, 1.0-texCutRatioH));
        mesh.buffer.addVector2(attTexcoord, Vector2(1.0-texCutRatioW, 1.0-texCutRatioH));
        mesh.buffer.addVector2(attTexcoord, Vector2(1.0-texCutRatioW, texCutRatioH));
        mesh.buffer.addVector2(attTexcoord, Vector2(texCutRatioW, texCutRatioH));
    }

    Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);

    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));

    Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);

    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));


    static const uint16_t indices_array[] = {
        0,  1,  2,
        0,  2,  3,
    };

    mesh.indices.setValues(
        0, mesh.indices.getAttribute(AttributeType::INDEX),
        6, (char*)&indices_array[0], sizeof(uint16_t));

    if (mesh.loaded)
        mesh.needUpdateBuffer = true;
}

void MeshSystem::createMeshPolygon(MeshPolygonComponent& polygon, MeshComponent& mesh){
    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLE_STRIP;
    mesh.numSubmeshes = 1;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    for (int i = 0; i < polygon.points.size(); i++){
        mesh.buffer.addVector3(AttributeType::POSITION, polygon.points[i].position);
        mesh.buffer.addVector3(AttributeType::NORMAL, Vector3(0.0f, 0.0f, 1.0f));
        mesh.buffer.addVector4(AttributeType::COLOR, polygon.points[i].color);
    }

    // Generation texcoords
    float min_X = std::numeric_limits<float>::max();
    float max_X = std::numeric_limits<float>::min();
    float min_Y = std::numeric_limits<float>::max();
    float max_Y = std::numeric_limits<float>::min();

    Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);

    for (unsigned int i = 0; i < mesh.buffer.getCount(); i++){
        min_X = fmin(min_X, mesh.buffer.getFloat(attVertex, i, 0));
        min_Y = fmin(min_Y, mesh.buffer.getFloat(attVertex, i, 1));
        max_X = fmax(max_X, mesh.buffer.getFloat(attVertex, i, 0));
        max_Y = fmax(max_Y, mesh.buffer.getFloat(attVertex, i, 1));
    }

    double k_X = 1/(max_X - min_X);
    double k_Y = 1/(max_Y - min_Y);

    float u = 0;
    float v = 0;

    for ( unsigned int i = 0; i < mesh.buffer.getCount(); i++){
        u = (mesh.buffer.getFloat(attVertex, i, 0) - min_X) * k_X;
        v = (mesh.buffer.getFloat(attVertex, i, 1) - min_Y) * k_Y;
        if (polygon.flipY) {
            mesh.buffer.addVector2(AttributeType::TEXCOORD1, Vector2(u, 1.0 - v));
        }else{
            mesh.buffer.addVector2(AttributeType::TEXCOORD1, Vector2(u, v));
        }
    }

    polygon.width = (int)(max_X - min_X);
    polygon.height = (int)(max_Y - min_Y);

    if (mesh.loaded)
        mesh.needUpdateBuffer = true;
}

void MeshSystem::createTilemap(TilemapComponent& tilemap, MeshComponent& mesh){
    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLES;
    mesh.submeshes[0].hasTextureRect = true;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    mesh.buffer.setUsage(BufferUsage::DYNAMIC);

    mesh.indices.clear();

    tilemap.width = 0;
    tilemap.height = 0;

    std::vector<std::vector<uint16_t>> indexMap;
    indexMap.resize(mesh.numSubmeshes);

    unsigned int numTiles = 0;
    unsigned int reserveTiles = tilemap.reserveTiles;

    for (int i = 0; i < MAX_TILEMAP_TILES; i++){

        if (tilemap.tiles[i].width == 0 && tilemap.tiles[i].height == 0 && reserveTiles == 0){
            continue;
        }

        if (tilemap.tiles[i].rectId < 0 || tilemap.tiles[i].rectId >= MAX_TILEMAP_TILESRECT){
            continue;
        }

        if (reserveTiles > 0){
            reserveTiles--;
        }

        numTiles++;

        Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);
        mesh.buffer.addVector3(attVertex, Vector3(tilemap.tiles[i].position.x, tilemap.tiles[i].position.y, 0));
        mesh.buffer.addVector3(attVertex, Vector3(tilemap.tiles[i].position.x + tilemap.tiles[i].width, tilemap.tiles[i].position.y, 0));
        mesh.buffer.addVector3(attVertex, Vector3(tilemap.tiles[i].position.x + tilemap.tiles[i].width, tilemap.tiles[i].position.y + tilemap.tiles[i].height, 0));
        mesh.buffer.addVector3(attVertex, Vector3(tilemap.tiles[i].position.x, tilemap.tiles[i].position.y + tilemap.tiles[i].height, 0));

        if (tilemap.width < tilemap.tiles[i].position.x + tilemap.tiles[i].width)
            tilemap.width = tilemap.tiles[i].position.x + tilemap.tiles[i].width;
        if (tilemap.height < tilemap.tiles[i].position.y + tilemap.tiles[i].height)
            tilemap.height = tilemap.tiles[i].position.y + tilemap.tiles[i].height;

        TileRectData& rectData = tilemap.tilesRect[tilemap.tiles[i].rectId];
        Rect& tileRect = rectData.rect;

        Texture& texture = mesh.submeshes[rectData.submeshId].material.baseColorTexture;
        Texture& mainTexture = mesh.submeshes[0].material.baseColorTexture;

        unsigned int texWidth = 0;
        unsigned int texHeight = 0;
        if (texture.load()){
            tileRect = normalizeTileRect(tileRect, texture.getWidth(), texture.getHeight());
            texWidth = texture.getWidth();
            texHeight = texture.getHeight();
        }else if (mainTexture.load()){
            tileRect = normalizeTileRect(tileRect, mainTexture.getWidth(), mainTexture.getHeight());
            texWidth = mainTexture.getWidth();
            texHeight = mainTexture.getHeight();
        }

        float texCutRatioW = 0;
        float texCutRatioH = 0;
        if (texWidth != 0 && texHeight != 0){
            texCutRatioW = 1.0 / texWidth * tilemap.textureCutFactor;
            texCutRatioH = 1.0 / texHeight * tilemap.textureCutFactor;
        }

        Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);
        if (tilemap.flipY){
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+texCutRatioW, tileRect.getY()+tileRect.getHeight()-texCutRatioH));
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+tileRect.getWidth()-texCutRatioW, tileRect.getY()+tileRect.getHeight()-texCutRatioH));
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+tileRect.getWidth()-texCutRatioW, tileRect.getY()+texCutRatioH));
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+texCutRatioW, tileRect.getY()+texCutRatioH));
        }else{
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+texCutRatioW, tileRect.getY()+texCutRatioH));
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+tileRect.getWidth()-texCutRatioW, tileRect.getY()+texCutRatioH));
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+tileRect.getWidth()-texCutRatioW, tileRect.getY()+tileRect.getHeight()-texCutRatioH));
            mesh.buffer.addVector2(attTexcoord, Vector2(tileRect.getX()+texCutRatioW, tileRect.getY()+tileRect.getHeight()-texCutRatioH));
        }

        Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);
        mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
        mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
        mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
        mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));

        Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

        indexMap[rectData.submeshId].push_back(0 + (i*4));
        indexMap[rectData.submeshId].push_back(1 + (i*4));
        indexMap[rectData.submeshId].push_back(2 + (i*4));
        indexMap[rectData.submeshId].push_back(0 + (i*4));
        indexMap[rectData.submeshId].push_back(2 + (i*4));
        indexMap[rectData.submeshId].push_back(3 + (i*4));

    }

    for (int i = 0; i < mesh.numSubmeshes; i++){
        addSubmeshAttribute(mesh.submeshes[i], "indices", AttributeType::INDEX, 1, AttributeDataType::UNSIGNED_SHORT, indexMap[i].size(), mesh.indices.getCount() * sizeof(uint16_t), false);

        mesh.indices.setValues(mesh.indices.getCount(), mesh.indices.getAttribute(AttributeType::INDEX), indexMap[i].size(), (char*)&indexMap[i].front(), sizeof(uint16_t));
        mesh.indices.setRenderAttributes(false);

        //TODO: necessary?
        //if (indexMap[i].size() == 0) {
        //     submesh not visible
        //} else {
        //     submesh visible
        //}
    }

    if (mesh.loaded){
        if (tilemap.numTiles < numTiles){
            mesh.needReload = true;
        }else{
            mesh.needUpdateBuffer = true;
        }
    }

    tilemap.numTiles = numTiles;
}

void MeshSystem::changeFlipY(bool& flipY, CameraComponent& camera, MeshComponent& mesh){
    flipY = false;
    if (camera.type != CameraType::CAMERA_2D){
        flipY = !flipY;
    }

    if (mesh.submeshes[0].material.baseColorTexture.isFramebuffer() && Engine::isOpenGL()){
        flipY = !flipY;
    }
}

Rect MeshSystem::normalizeTileRect(Rect tileRect, unsigned int texWidth, unsigned int texHeight){
    Rect normalized = tileRect;

    if (!tileRect.isNormalized()){
        normalized.setRect((tileRect.getX()) / (float) texWidth,
                            (tileRect.getY()) / (float) texHeight,
                            (tileRect.getWidth()) / (float) texWidth,
                            (tileRect.getHeight()) / (float) texHeight);
    }

    return normalized;
}

std::vector<float> MeshSystem::getCylinderSideNormals(float baseRadius, float topRadius, float height, float slices){
    float sectorStep = 2 * M_PI / slices;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float yAngle = atan2(baseRadius - topRadius, height);
    float x0 = cos(yAngle);     // nx
    float y0 = sin(yAngle);     // ny
    float z0 = 0;               // nz

    // rotate (x0,y0,z0) per sector angle
    std::vector<float> normals;
    for(int i = 0; i <= slices; ++i)
    {
        sectorAngle = i * sectorStep;
        normals.push_back(cos(sectorAngle)*x0 - sin(sectorAngle)*z0);   // nx
        normals.push_back(y0);   // ny
        normals.push_back(sin(sectorAngle)*x0 + cos(sectorAngle)*z0);  // nz
    }

    return normals;
}

// generate 3D vertices of a unit circle on XZ plane
std::vector<float> MeshSystem::buildUnitCircleVertices(float slices){
    float sectorStep = 2 * M_PI / slices;
    float sectorAngle;  // radian

    std::vector<float> unitCircleVertices;

    for(int i = 0; i <= slices; ++i){
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(0);                // y
        unitCircleVertices.push_back(sin(sectorAngle)); // z
    }

    return unitCircleVertices;
}

std::string MeshSystem::readFileToString(const char* filename){
    Data filedata;

    if (filedata.open(filename) != FileErrors::FILEDATA_OK){
        Log::error("Model file not found: %s", filename);
        return "";
    }
    filedata.seek(0);
    return filedata.readString(filedata.length());
}

bool MeshSystem::fileExists(const std::string &abs_filename, void *) {
    File df;
    int res = df.open(abs_filename.c_str());

    if (!res) {
        return true;
    }

    return false;
}

bool MeshSystem::readWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *) {
    Data filedata;

    if (filedata.open(filepath.c_str()) != FileErrors::FILEDATA_OK){
        Log::error("Model file not found: %s", filepath.c_str());
        return false;
    }

    filedata.seek(0);
    std::istringstream f(filedata.readString(filedata.length()));

    if (!f) {
        if (err) {
            (*err) += "File open error : " + filepath + "\n";
        }
        return false;
    }

    f.seekg(0, f.end);
    size_t sz = static_cast<size_t>(f.tellg());
    f.seekg(0, f.beg);

    if (int(sz) < 0) {
        if (err) {
            (*err) += "Invalid file size : " + filepath +
                      " (does the path point to a directory?)";
        }
        return false;
    } else if (sz == 0) {
        if (err) {
            (*err) += "File is empty : " + filepath + "\n";
        }
        return false;
    }

    out->resize(sz);
    f.read(reinterpret_cast<char *>(&out->at(0)),
           static_cast<std::streamsize>(sz));
    //f.close();

    return true;
}

void MeshSystem::addSubmeshAttribute(Submesh& submesh, std::string bufferName, AttributeType attribute, unsigned int elements, AttributeDataType dataType, size_t size, size_t offset, bool normalized){
    Attribute attData;

    attData.setBuffer(bufferName);
    attData.setDataType(dataType);
    attData.setElements(elements);
    attData.setCount(size);
    attData.setOffset(offset);
    attData.setNormalized(normalized);

    submesh.attributes[attribute] = attData;
}

bool MeshSystem::loadGLTFBuffer(int bufferViewIndex, MeshComponent& mesh, ModelComponent& model, const int stride, std::vector<std::string>& loadedBuffers){
    const tinygltf::BufferView &bufferView = model.gltfModel->bufferViews[bufferViewIndex];
    const std::string name = getBufferName(bufferViewIndex, model);

    if (std::find(loadedBuffers.begin(), loadedBuffers.end(), name) == loadedBuffers.end() && bufferView.target != 0) {
        loadedBuffers.push_back(name);

        if (bufferView.target == 34962) { //GL_ARRAY_BUFFER
            mesh.eBuffers[mesh.numExternalBuffers].setType(BufferType::VERTEX_BUFFER);
        } else if (bufferView.target == 34963) { //GL_ELEMENT_ARRAY_BUFFER
            mesh.eBuffers[mesh.numExternalBuffers].setType(BufferType::INDEX_BUFFER);
        }

        mesh.eBuffers[mesh.numExternalBuffers].setData(&model.gltfModel->buffers[bufferView.buffer].data.at(0) + bufferView.byteOffset, bufferView.byteLength);
        mesh.eBuffers[mesh.numExternalBuffers].setStride(stride);
        mesh.eBuffers[mesh.numExternalBuffers].setName(name);
        mesh.eBuffers[mesh.numExternalBuffers].setRenderAttributes(false);

        mesh.numExternalBuffers++;
        if (mesh.numExternalBuffers > MAX_EXTERNAL_BUFFERS){
            Log::error("External buffer limit reached for GLTF model");
        }

        return true;
    }

    return false;
}

bool MeshSystem::loadGLTFTexture(int textureIndex, ModelComponent& model, Texture& texture, std::string textureName){
    if (textureIndex >= 0){
        tinygltf::Texture &tex = model.gltfModel->textures[textureIndex];
        tinygltf::Image &image = model.gltfModel->images[tex.source];

        size_t imageSize = image.component * image.width * image.height; //in bytes

        ColorFormat colorFormat;
        if (image.component == 1){
            colorFormat = ColorFormat::RED;
        }else if (image.component == 4){
            colorFormat = ColorFormat::RGBA;
        }else{
            Log::error("Not compatible image %i: Renders only support 8bpp and 32bpp", textureName.c_str());
            return false;
        }

        TextureData textureData(image.width, image.height, imageSize, colorFormat, image.component, &image.image.at(0));

        std::string id = textureName + "|" + image.name;
        texture.setData(textureData, id);

        if (tex.sampler >= 0){
            tinygltf::Sampler &sampler = model.gltfModel->samplers[tex.sampler];
            texture.setMinFilter(convertFilter(sampler.minFilter));
            texture.setMagFilter(convertFilter(sampler.magFilter));
            texture.setWrapU(convertWrap(sampler.wrapS));
            texture.setWrapV(convertWrap(sampler.wrapT));
        }
        // Prevent GLTF release because GLTF can have multiple textures with the same data
        // Image data is stored in tinygltf::Image
        texture.setReleaseDataAfterLoad(false);
    }

    return true;
}

std::string MeshSystem::getBufferName(int bufferViewIndex, ModelComponent& model){
    const tinygltf::BufferView &bufferView = model.gltfModel->bufferViews[bufferViewIndex];

    if (!bufferView.name.empty())
        return bufferView.name;
    else
        return "buffer"+std::to_string(bufferViewIndex);

}

Matrix4 MeshSystem::getGLTFNodeMatrix(int nodeIndex, ModelComponent& model){
    tinygltf::Node node = model.gltfModel->nodes[nodeIndex];

    Matrix4 matrix;

    if (node.matrix.size() == 16){
        matrix = Matrix4(
                node.matrix[0],node.matrix[4],node.matrix[8], node.matrix[12],
                node.matrix[1],node.matrix[5],node.matrix[9], node.matrix[13],
                node.matrix[2],node.matrix[6],node.matrix[10],node.matrix[14],
                node.matrix[3],node.matrix[7],node.matrix[11],node.matrix[15]);
    }else{

        Vector3 translation;
        Vector3 scale;
        Quaternion rotation;

        if (node.translation.size() == 3){
            translation = Vector3(node.translation[0], node.translation[1], node.translation[2]);
        }else{
            translation = Vector3(0.0, 0.0, 0.0);
        }
        if (node.rotation.size() == 4){
            rotation = Quaternion(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
        }else{
            rotation = Quaternion(1.0, 0.0, 0.0, 0.0);
        }
        if (node.scale.size() == 3){
            scale = Vector3(node.scale[0], node.scale[1], node.scale[2]);
        }else{
            scale = Vector3(1.0, 1.0, 1.0);
        }

        Matrix4 scaleMatrix = Matrix4::scaleMatrix(scale);
        Matrix4 translateMatrix = Matrix4::translateMatrix(translation);
        Matrix4 rotationMatrix = rotation.getRotationMatrix();

        matrix = translateMatrix * rotationMatrix * scaleMatrix;
    }

    return matrix;
}

Matrix4 MeshSystem::getGLTFMeshGlobalMatrix(int nodeIndex, ModelComponent& model, std::map<int, int>& nodesParent){

    Matrix4 matrix = getGLTFNodeMatrix(nodeIndex, model);

    int parent = nodesParent[nodeIndex];

    if (parent >= 0){
        return getGLTFMeshGlobalMatrix(parent, model, nodesParent) * matrix;
    }

    return matrix;
}

Entity MeshSystem::generateSketetalStructure(Entity entity, ModelComponent& model, int nodeIndex, int skinIndex){
    tinygltf::Node node = model.gltfModel->nodes[nodeIndex];
    tinygltf::Skin skin = model.gltfModel->skins[skinIndex];

    int index = -1;

    for (int j = 0; j < skin.joints.size(); j++) {
        if (nodeIndex == skin.joints[j])
            index = j;
    }

    Matrix4 offsetMatrix;

    if (skin.inverseBindMatrices >= 0) {

        tinygltf::Accessor accessor = model.gltfModel->accessors[skin.inverseBindMatrices];
        tinygltf::BufferView bufferView = model.gltfModel->bufferViews[accessor.bufferView];

        float *matrices = (float *) (&model.gltfModel->buffers[bufferView.buffer].data.at(0) +
                                     bufferView.byteOffset + accessor.byteOffset +
                                     (16 * sizeof(float) * index));

        if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT || accessor.type != TINYGLTF_TYPE_MAT4) {
            Log::error("Skeleton error: Unknown inverse bind matrix data type");

            return NULL_ENTITY;
        }

        offsetMatrix = Matrix4(
                matrices[0], matrices[4], matrices[8], matrices[12],
                matrices[1], matrices[5], matrices[9], matrices[13],
                matrices[2], matrices[6], matrices[10], matrices[14],
                matrices[3], matrices[7], matrices[11], matrices[15]);

    }

    Entity bone;

    bone = scene->createEntity();
    scene->addComponent<Transform>(bone, {});
    scene->addComponent<BoneComponent>(bone, {});

    Transform& bonetransform = scene->getComponent<Transform>(bone);
    BoneComponent& bonecomp = scene->getComponent<BoneComponent>(bone);

    bonecomp.index = index;
    bonetransform.name = node.name;

    Matrix4 matrix = getGLTFNodeMatrix(nodeIndex, model);
    matrix.decompose(bonecomp.bindPosition, bonecomp.bindScale, bonecomp.bindRotation);

    // move to bind
    bonetransform.position = bonecomp.bindPosition;
    bonetransform.rotation = bonecomp.bindRotation;
    bonetransform.scale = bonecomp.bindScale;

    bonecomp.offsetMatrix = offsetMatrix;
    bonecomp.model = entity;

    model.bonesNameMapping[bonetransform.name] = bone;
    model.bonesIdMapping[nodeIndex] = bone;

    for (size_t i = 0; i < node.children.size(); i++){
        // here bonetransform and bonecomp losts references
        scene->addEntityChild(bone, generateSketetalStructure(entity, model, node.children[i], skinIndex));
    }

    return bone;
}

TextureFilter MeshSystem::convertFilter(int filter){
    if (filter==TINYGLTF_TEXTURE_FILTER_NEAREST){
        return TextureFilter::NEAREST;
    }else if (filter==TINYGLTF_TEXTURE_FILTER_LINEAR){
        return TextureFilter::LINEAR;
    }else if (filter==TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST){
        return TextureFilter::NEAREST_MIPMAP_NEAREST;
    }else if (filter==TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST){
        return TextureFilter::LINEAR_MIPMAP_NEAREST;
    }else if (filter==TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR){
        return TextureFilter::NEAREST_MIPMAP_LINEAR;
    }else if (filter==TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR){
        return TextureFilter::LINEAR_MIPMAP_LINEAR;
    }

    return TextureFilter::NEAREST;
}

TextureWrap MeshSystem::convertWrap(int wrap){
    if (wrap==TINYGLTF_TEXTURE_WRAP_REPEAT){
        return TextureWrap::REPEAT;
    }else if (wrap==TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE){
        return TextureWrap::CLAMP_TO_EDGE;
    }else if (wrap==TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT){
        return TextureWrap::MIRRORED_REPEAT;
    }

    return TextureWrap::REPEAT;
}

void MeshSystem::clearAnimations(ModelComponent& model){
    for (int i = 0; i < model.animations.size(); i++){
        scene->destroyEntity(model.animations[i]);
    }
    model.animations.clear();
}


TerrainNodeIndex MeshSystem::createPlaneNodeBuffer(TerrainComponent& terrain, int width, int height, int widthSegments, int heightSegments){
    float width_half = (float)width / 2;
    float height_half = (float)height / 2;

    int gridX = widthSegments;
    int gridY = heightSegments;

    int gridX1 = gridX + 1;
    int gridY1 = gridY + 1;

    float segment_width = (float)width / gridX;
    float segment_height = (float)height / gridY;

    Attribute* attVertex = terrain.buffer.getAttribute(AttributeType::POSITION);
    Attribute* attNormal = terrain.buffer.getAttribute(AttributeType::NORMAL);

    Attribute* attIndice = terrain.indices.getAttribute(AttributeType::INDEX);

    int bufferCount = terrain.buffer.getCount();

    for (int iy = 0; iy < gridY1; iy++) {
        float y = iy * segment_height - height_half;
        for (int ix = 0; ix < gridX1; ix ++) {

            float x = ix * segment_width - width_half;

            terrain.buffer.addVector3(attVertex, Vector3(x, 0, -y));
            terrain.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
        }
    }

    unsigned int bufferIndexCount = 0;
    unsigned int bufferIndexOffset = terrain.indices.getCount();

    for (int iy = 0; iy < gridY; iy++) {
        for (int ix = 0; ix < gridX; ix++) {

            int a = ix + gridX1 * iy;
            int b = ix + gridX1 * ( iy + 1 );
            int c = ( ix + 1 ) + gridX1 * ( iy + 1 );
            int d = ( ix + 1 ) + gridX1 * iy;

            terrain.indices.addUInt16(attIndice, a + bufferCount);
            terrain.indices.addUInt16(attIndice, b + bufferCount);
            terrain.indices.addUInt16(attIndice, d + bufferCount);

            terrain.indices.addUInt16(attIndice, b + bufferCount);
            terrain.indices.addUInt16(attIndice, c + bufferCount);
            terrain.indices.addUInt16(attIndice, d + bufferCount);

            bufferIndexCount += 6;

        }
    }

    return {bufferIndexCount, bufferIndexOffset};
}

size_t MeshSystem::getTerrainGridArraySize(int rootGridSize, int levels){
    size_t size = 0;
    for (int i = 0; i<=(levels-1); i++){
        size += pow(4, i);
    }
    size = size * rootGridSize * rootGridSize;

    return size;
}

float MeshSystem::getTerrainHeight(TerrainComponent& terrain, float x, float y){

    if (x < 0 || y < 0 || x >= terrain.terrainSize || y >= terrain.terrainSize)
        return 0;

    TextureData& textureData = terrain.heightMap.getData();

    int posX = floor(textureData.getWidth() * x / terrain.terrainSize);
    int posY = floor(textureData.getHeight() * y / terrain.terrainSize);

    float val = terrain.maxHeight*(textureData.getColorComponent(posX,posY,0)/255.0f);
    return val;
}

float MeshSystem::maxTerrainHeightArea(TerrainComponent& terrain, float x, float z, float w, float h) {
    float maxVal = std::numeric_limits<float>::min();
    for(float i = x; i < x+w; i++)
        for(float j = z; j < z+h; j++){
            float newVal = getTerrainHeight(terrain, i,j);
            if(newVal > maxVal) {
                maxVal = newVal;
            }
        }
    return maxVal;
}

float MeshSystem::minTerrainHeightArea(TerrainComponent& terrain, float x, float z, float w, float h) {
    float minVal = std::numeric_limits<float>::max();
    for(float i = x; i < x+w; i++)
        for(float j = z; j < z+h; j++){
            float newVal = getTerrainHeight(terrain, i,j);
            if(newVal < minVal) {
                minVal = newVal;
            }
        }
    return minVal;
}

void MeshSystem::createTerrainNode(TerrainComponent& terrain, float x, float y, float size, int lodDepth){
    TerrainNode& node = terrain.nodes[terrain.numNodes++];

    node.position = Vector2(x, y);
    node.size = size;

    node.currentRange = 0;
    node.resolution = terrain.resolution;

    float halfSize = size/2;

    if (lodDepth == 1){
        float terrainHalfSize = terrain.terrainSize / 2;
        float relativeX = x - halfSize + terrainHalfSize;
        float relativeY = y - halfSize + terrainHalfSize;

        node.hasChilds = false;

        node.maxHeight = maxTerrainHeightArea(terrain, relativeX, relativeY, size, size);
        node.minHeight = minTerrainHeightArea(terrain, relativeX, relativeY, size, size);
    }else{
        float quarterSize = halfSize/2;

        node.childs[0] = terrain.numNodes;
        createTerrainNode(terrain, x - quarterSize, y - quarterSize, halfSize, lodDepth - 1);
        terrain.nodes[node.childs[0]].visible = false;

        node.childs[1] = terrain.numNodes;
        createTerrainNode(terrain, x - quarterSize, y + quarterSize, halfSize, lodDepth - 1);
        terrain.nodes[node.childs[1]].visible = false;

        node.childs[2] = terrain.numNodes;
        createTerrainNode(terrain, x + quarterSize, y - quarterSize, halfSize, lodDepth - 1);
        terrain.nodes[node.childs[2]].visible = false;

        node.childs[3] = terrain.numNodes;
        createTerrainNode(terrain, x + quarterSize, y + quarterSize, halfSize, lodDepth - 1);
        terrain.nodes[node.childs[3]].visible = false;

        node.hasChilds = true;

        node.maxHeight = std::max(std::max(terrain.nodes[node.childs[0]].maxHeight, terrain.nodes[node.childs[1]].maxHeight), std::max(terrain.nodes[node.childs[2]].maxHeight, terrain.nodes[node.childs[3]].maxHeight));
        node.minHeight = std::min(std::min(terrain.nodes[node.childs[0]].minHeight, terrain.nodes[node.childs[1]].minHeight), std::min(terrain.nodes[node.childs[2]].minHeight, terrain.nodes[node.childs[3]].minHeight));
    }
}

void MeshSystem::createTerrain(TerrainComponent& terrain){
    terrain.buffer.clear();
    terrain.buffer.addAttribute(AttributeType::POSITION, 3);
	terrain.buffer.addAttribute(AttributeType::NORMAL, 3);

    if (scene->getCamera() == NULL_ENTITY){
        Log::error("Cannot create terrain without defined camera in scene");
        return;
    }

    if (MAX_TERRAINGRID < (terrain.rootGridSize*terrain.rootGridSize)){
        Log::error("Cannot create full terrain, increase MAX_TERRAINGRID to %u", (terrain.rootGridSize*terrain.rootGridSize));
        return;
    }

    terrain.indices.clear();

    terrain.heightMap.setReleaseDataAfterLoad(false);

    if (!terrain.heightMap.load()){
        Log::error("Terrain must have a heightmap");
        return;
    }

    size_t idealSize = getTerrainGridArraySize(terrain.rootGridSize, terrain.levels);
    terrain.nodes.resize(idealSize);

    terrain.fullResNode = createPlaneNodeBuffer(terrain, 1, 1, terrain.resolution, terrain.resolution);
    terrain.halfResNode = createPlaneNodeBuffer(terrain, 1, 1, terrain.resolution/2, terrain.resolution/2);

    float rootNodeSize = terrain.terrainSize / terrain.rootGridSize;

    if (terrain.autoSetRanges) {
        float maxDistance = 100;
        CameraComponent& camera =  scene->getComponent<CameraComponent>(scene->getCamera());
        if (camera.type == CameraType::CAMERA_PERSPECTIVE){
            maxDistance = camera.perspectiveFar;
        }else{
            maxDistance = camera.orthoFar;
        }

        float lastLevel = maxDistance;
        if (maxDistance < (rootNodeSize * 2)){
            lastLevel = rootNodeSize * 2;
            Log::warn("Terrain quadtree root is not in camera field of view. Increase terrain root grid.");
        }

        terrain.ranges.clear();
        terrain.ranges.resize(terrain.levels);
        terrain.ranges[terrain.levels - 1] = lastLevel;
        for (int i = terrain.levels - 2; i >= 0; i--) {
            terrain.ranges[i] = terrain.ranges[i + 1] / 2;
        }
    }

    terrain.numNodes = 0;

    // center terrain
    float offset = (terrain.terrainSize / 2) - (rootNodeSize / 2);

    terrain.numNodes = 0;
    for (int i = 0; i < terrain.rootGridSize; i++) {
        for (int j = 0; j < terrain.rootGridSize; j++) {
            float xPos = i*rootNodeSize;
            float zPos = j*rootNodeSize;

            terrain.grid[j + i*terrain.rootGridSize] = terrain.numNodes;
            createTerrainNode(terrain, xPos-offset, zPos-offset, rootNodeSize, terrain.levels);
        }
    }

    terrain.heightMapLoaded = true;

}

void MeshSystem::createPlane(Entity entity, float width, float depth, unsigned int tiles){
    MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLES;
    mesh.numSubmeshes = 1;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    mesh.indices.clear();

    float halfWidth = width / 2.0;
    float halfDepth = depth / 2.0;

    Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, 0, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, 0, halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, 0, halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, 0, -halfDepth));

    Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);
    mesh.buffer.addVector2(attTexcoord, Vector2(0.0f, 0.0f));
    mesh.buffer.addVector2(attTexcoord, Vector2(0.0f, 1.0f * tiles));
    mesh.buffer.addVector2(attTexcoord, Vector2(1.0f * tiles, 1.0f * tiles));
    mesh.buffer.addVector2(attTexcoord, Vector2(1.0f * tiles, 0.0f));

    Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));

    Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    static const uint16_t indices_array[] = {
        0,  1,  2,
        0,  2,  3,
    };

    mesh.indices.setValues(
        0, mesh.indices.getAttribute(AttributeType::INDEX),
        6, (char*)&indices_array[0], sizeof(uint16_t));
}

void MeshSystem::createBox(Entity entity, float width, float height, float depth, unsigned int tiles){
    MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLES;
    mesh.numSubmeshes = 1;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    mesh.indices.clear();

    float halfWidth = width / 2.0;
    float halfHeight = height / 2.0;
    float halfDepth = depth / 2.0;

    Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);

    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, -halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, -halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth,  halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, halfHeight,  halfDepth));

    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, -halfHeight, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, -halfHeight, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth,  halfHeight, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth,  halfHeight, -halfDepth));

    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, -halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth,  halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth,  halfHeight, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, -halfHeight, -halfDepth));

    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, -halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth,  halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth,  halfHeight, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, -halfHeight, -halfDepth));

    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth,  halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth,  halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth,  halfHeight, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth,  halfHeight, -halfDepth));

    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, -halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, -halfHeight,  halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(halfWidth, -halfHeight, -halfDepth));
    mesh.buffer.addVector3(attVertex, Vector3(-halfWidth, -halfHeight, -halfDepth));

    Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);

    for (int i = 0; i < 6; i++){
        mesh.buffer.addVector2(attTexcoord, Vector2(0.0f, 0.0f));
        mesh.buffer.addVector2(attTexcoord, Vector2(1.0f * tiles, 0.0f));
        mesh.buffer.addVector2(attTexcoord, Vector2(1.0f * tiles, 1.0f * tiles));
        mesh.buffer.addVector2(attTexcoord, Vector2(0.0f, 1.0f * tiles));
    }

    Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);

    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, 1.0f));
    
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, -1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, -1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, -1.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 0.0f, -1.0f));
    
    mesh.buffer.addVector3(attNormal, Vector3(-1.0f, 0.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(-1.0f, 0.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(-1.0f, 0.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(-1.0f, 0.0f, 0.0f));
    
    mesh.buffer.addVector3(attNormal, Vector3(1.0f, 0.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(1.0f, 0.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(1.0f, 0.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(1.0f, 0.0f, 0.0f));
    
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, 1.0f, 0.0f));
    
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, -1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, -1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, -1.0f, 0.0f));
    mesh.buffer.addVector3(attNormal, Vector3(0.0f, -1.0f, 0.0f));

    Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);

    for (int i = 0; i < 6; i++){
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    static const uint16_t indices_array[] = {
            /* front */
            0,  1,  2,
            0,  2,  3,
            /* back */
            4,  5,  6,
            4,  6,  7,
            /* left */
            8,  9, 10,
            8, 10, 11,
            /* right */
            12, 13, 14,
            12, 14, 15,
            /* top */
            16, 17, 18,
            16, 18, 19,
            /* bottom */
            20, 21, 22,
            20, 22, 23,
    };

    mesh.indices.setValues(
        0, mesh.indices.getAttribute(AttributeType::INDEX),
        36, (char*)&indices_array[0], sizeof(uint16_t));
}

void MeshSystem::createSphere(Entity entity, float radius, unsigned int slices, unsigned int stacks){
    MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLES;
    mesh.numSubmeshes = 1;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);
    Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);
    Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);
    Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);

    mesh.indices.clear();

    float x, y, z, xz;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * M_PI / slices;
    float stackStep = M_PI / stacks;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stacks; ++i){
        stackAngle = M_PI / 2 - i * stackStep;      // starting from pi/2 to -pi/2
        xz = radius * cosf(stackAngle);             // r * cos(u)
        y = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= slices; ++j){
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xz * cosf(sectorAngle);             // r * cos(u) * cos(v)
            z = xz * sinf(sectorAngle);             // r * cos(u) * sin(v)
            mesh.buffer.addVector3(attVertex, Vector3(x, y, z));

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            mesh.buffer.addVector3(attNormal, Vector3(nx, ny, nz));

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / slices;
            t = (float)i / stacks;
            mesh.buffer.addVector2(attTexcoord, Vector2(s, t));

            // vertex color (white)
            mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    // generate CCW index list of sphere triangles
    std::vector<uint16_t> indices;
    int k1, k2;
    for(int i = 0; i < stacks; ++i){
        k1 = i * (slices + 1);     // beginning of current stack
        k2 = k1 + slices + 1;      // beginning of next stack

        for(int j = 0; j < slices; ++j, ++k1, ++k2){
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0){
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stacks-1)){
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    mesh.indices.setValues(
        0, mesh.indices.getAttribute(AttributeType::INDEX),
        indices.size(), (char*)&indices[0], sizeof(uint16_t));
}

void MeshSystem::createCylinder(Entity entity, float baseRadius, float topRadius, float height, unsigned int slices, unsigned int stacks){
    MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLES;
    mesh.numSubmeshes = 1;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);
    Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);
    Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);
    Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);

    float x, y, z;                                  // vertex position
    float radius;                                   // radius for each stack

    std::vector<float> sideNormals = getCylinderSideNormals(baseRadius, topRadius, height, slices);
    std::vector<float> unitCircleVertices = buildUnitCircleVertices(slices);

    // put vertices of side cylinder to array by scaling unit circle
    for(int i = 0; i <= stacks; ++i){
        y = -(height * 0.5f) + (float)i / stacks * height;      // vertex position y
        radius = baseRadius + (float)i / stacks * (topRadius - baseRadius);     // lerp
        float t = 1.0f - (float)i / stacks;   // top-to-bottom

        for(int j = 0, k = 0; j <= slices; ++j, k += 3){
            x = unitCircleVertices[k];
            z = unitCircleVertices[k+2];
            mesh.buffer.addVector3(attVertex, Vector3(x * radius, y, z * radius));
            mesh.buffer.addVector3(attNormal, Vector3(sideNormals[k], sideNormals[k+1], sideNormals[k+2]));
            mesh.buffer.addVector2(attTexcoord, Vector2((float)j / slices, t));
            mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    // remember where the base.top vertices start
    unsigned int baseVertexIndex = mesh.buffer.getCount();

    // put vertices of base of cylinder
    y = -height * 0.5f;
    mesh.buffer.addVector3(attVertex, Vector3(0, y, 0));
    mesh.buffer.addVector3(attNormal, Vector3(0, -1, 0));
    mesh.buffer.addVector2(attTexcoord, Vector2(0.5f, 0.5f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    for(int i = 0, j = 0; i < slices; ++i, j += 3){
        x = unitCircleVertices[j];
        z = unitCircleVertices[j+2];
        mesh.buffer.addVector3(attVertex, Vector3(x * baseRadius, y, z * baseRadius));
        mesh.buffer.addVector3(attNormal, Vector3(0, -1, 0));
        mesh.buffer.addVector2(attTexcoord, Vector2(-x * 0.5f + 0.5f, -z * 0.5f + 0.5f)); // flip horizontal
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = mesh.buffer.getCount();

    // put vertices of top of cylinder
    y = height * 0.5f;
    mesh.buffer.addVector3(attVertex, Vector3(0, y, 0));
    mesh.buffer.addVector3(attNormal, Vector3(0, 1, 0));
    mesh.buffer.addVector2(attTexcoord, Vector2(0.5f, 0.5f));
    mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    for(int i = 0, j = 0; i < slices; ++i, j += 3){
        x = unitCircleVertices[j];
        z = unitCircleVertices[j+2];
        mesh.buffer.addVector3(attVertex, Vector3(x * topRadius, y, z * topRadius));
        mesh.buffer.addVector3(attNormal, Vector3(0, 1, 0));
        mesh.buffer.addVector2(attTexcoord, Vector2(x * 0.5f + 0.5f, -z * 0.5f + 0.5f));
        mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    std::vector<uint16_t> indices;

    // put indices for sides
    unsigned int k1, k2;
    for(int i = 0; i < stacks; ++i){
        k1 = i * (slices + 1);     // bebinning of current stack
        k2 = k1 + slices + 1;      // beginning of next stack

        for(int j = 0; j < slices; ++j, ++k1, ++k2){
            // 2 trianles per sector
            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            indices.push_back(k2);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
        }
    }

    // put indices for base
    for(int i = 0, k = baseVertexIndex + 1; i < slices; ++i, ++k){
        if(i < (slices - 1)){
            indices.push_back(baseVertexIndex);
            indices.push_back(k + 1);
            indices.push_back(k);
        }else{    // last triangle
            indices.push_back(baseVertexIndex);
            indices.push_back(baseVertexIndex + 1);
            indices.push_back(k);
        }
    }

    for(int i = 0, k = topVertexIndex + 1; i < slices; ++i, ++k)
    {
        if(i < (slices - 1)){
            indices.push_back(topVertexIndex);
            indices.push_back(k);
            indices.push_back(k + 1);
        }else{
            indices.push_back(topVertexIndex);
            indices.push_back(k);
            indices.push_back(topVertexIndex + 1);
        }
    }

    mesh.indices.setValues(
        0, mesh.indices.getAttribute(AttributeType::INDEX),
        indices.size(), (char*)&indices[0], sizeof(uint16_t));
}

void MeshSystem::createTorus(Entity entity, float radius, float ringRadius, unsigned int sides, unsigned int rings){
    MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

    mesh.submeshes[0].primitiveType = PrimitiveType::TRIANGLES;
    mesh.numSubmeshes = 1;

	mesh.buffer.clear();
	mesh.buffer.addAttribute(AttributeType::POSITION, 3);
	mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
	mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
    mesh.buffer.addAttribute(AttributeType::COLOR, 4);

    Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);
    Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);
    Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);
    Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);

    const float two_pi = 2.0f * M_PI;
    const float dv = 1.0f / sides;
    const float du = 1.0f / rings;

    // generate vertices
    for (uint32_t side = 0; side <= sides; side++) {
        const float phi = (side * two_pi) / sides;
        const float sin_phi = sinf(phi);
        const float cos_phi = cosf(phi);
        for (uint32_t ring = 0; ring <= rings; ring++) {
            const float theta = (ring * two_pi) / rings;
            const float sin_theta = sinf(theta);
            const float cos_theta = cosf(theta);

            // torus surface position
            const float spx = sin_theta * (radius - (ringRadius * cos_phi));
            const float spy = sin_phi * ringRadius;
            const float spz = cos_theta * (radius - (ringRadius * cos_phi));

            // torus position with ring-radius zero (for normal computation)
            const float ipx = sin_theta * radius;
            const float ipy = 0.0f;
            const float ipz = cos_theta * radius;

            mesh.buffer.addVector3(attVertex, Vector3(spx, spy, spz));
            mesh.buffer.addVector3(attNormal, Vector3(spx - ipx, spy - ipy, spz - ipz));
            mesh.buffer.addVector2(attTexcoord, Vector2(ring * du, 1.0f - side * dv));
            mesh.buffer.addVector4(attColor, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    // generate indices
    std::vector<uint16_t> indices;
    for (uint16_t side = 0; side < sides; side++) {
        const uint16_t row_a = side * (rings + 1);
        const uint16_t row_b = row_a + rings + 1;
        for (uint16_t ring = 0; ring < rings; ring++) {
            indices.push_back(row_a + ring);
            indices.push_back(row_a + ring + 1);
            indices.push_back(row_b + ring + 1);

            indices.push_back(row_a + ring);
            indices.push_back(row_b + ring + 1);
            indices.push_back(row_b + ring);
        }
    }

    mesh.indices.setValues(
        0, mesh.indices.getAttribute(AttributeType::INDEX),
        indices.size(), (char*)&indices[0], sizeof(uint16_t));
}

bool MeshSystem::loadGLTF(Entity entity, std::string filename){
    MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);
    ModelComponent& model = scene->getComponent<ModelComponent>(entity);
    Transform& transform = scene->getComponent<Transform>(entity);

    destroyModel(model);

    if (!model.gltfModel)
        model.gltfModel = new tinygltf::Model();

    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    int meshIndex = 0;
    std::vector<std::string> loadedBuffers;

    mesh.numExternalBuffers = 0;

    loader.SetFsCallbacks({&fileExists, &tinygltf::ExpandFilePath, &readWholeFile, nullptr, nullptr});
    //loader.SetFsCallbacks({nullptr, nullptr, nullptr, nullptr, nullptr});

    std::string ext = FileData::getFilePathExtension(filename);

    bool res = false;

    if (ext.compare("glb") == 0) {
        res = loader.LoadBinaryFromFile(model.gltfModel, &err, &warn, filename); // for binary glTF(.glb)
    }else{
        res = loader.LoadASCIIFromFile(model.gltfModel, &err, &warn, filename);
    }

    if (!warn.empty()) {
        Log::warn("Loading GLTF model (%s): %s", filename.c_str(), warn.c_str());
    }

    if (!err.empty()) {
        Log::error("Can't load GLTF model (%s): %s", filename.c_str(), err.c_str());
        return false;
    }

    if (!res) {
        Log::verbose("Failed to load glTF: %s", filename.c_str());
        return false;
    }

    int meshNode = -1;
    std::map<int, int> nodesParent;

    for (size_t i = 0; i < model.gltfModel->nodes.size(); i++) {
        nodesParent[i] = -1;
    }

    for (size_t i = 0; i < model.gltfModel->nodes.size(); i++) {
        tinygltf::Node node = model.gltfModel->nodes[i];

        if (node.mesh == meshIndex){
            meshNode = i;
        }

        for (int c = 0; c < node.children.size(); c++){
            nodesParent[node.children[c]] = i;
        }
    }

    // getting mesh transform based on GLTF scene root node
    Matrix4 matrix = getGLTFMeshGlobalMatrix(meshNode, model, nodesParent);
    matrix.decompose(transform.position, transform.scale, transform.rotation);
    transform.needUpdate = true;

    tinygltf::Mesh gltfmesh = model.gltfModel->meshes[meshIndex];

    mesh.numSubmeshes = gltfmesh.primitives.size();

    for (size_t i = 0; i < gltfmesh.primitives.size(); i++) {

        mesh.submeshes[i].attributes.clear();

        tinygltf::Primitive primitive = gltfmesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.gltfModel->accessors[primitive.indices];
        tinygltf::Material &mat = model.gltfModel->materials[primitive.material];

        AttributeDataType indexType;

        //Not supported TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE
        if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT){
            indexType = AttributeDataType::UNSIGNED_SHORT;
        }else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT){
            indexType = AttributeDataType::UNSIGNED_INT;
        }else{
            Log::error("Unknown index type %i", indexAccessor.componentType);
            continue;
        }

        if (!loadGLTFTexture(
            mat.pbrMetallicRoughness.baseColorTexture.index,
            model, 
            mesh.submeshes[i].material.baseColorTexture, 
            filename + "|" + "baseColorTexture"))
            continue;

        if (mat.pbrMetallicRoughness.baseColorTexture.texCoord != 0){
            Log::error("Not supported texcoord for %s, only one per submesh: %s", "baseColorTexture", filename.c_str());
            continue;
        }

        if (!loadGLTFTexture(
            mat.pbrMetallicRoughness.metallicRoughnessTexture.index, 
            model,
            mesh.submeshes[i].material.metallicRoughnessTexture, 
            filename + "|" + "metallicRoughnessTexture"))
            continue;

        if (mat.pbrMetallicRoughness.metallicRoughnessTexture.texCoord != 0){
            Log::error("Not supported texcoord for %s, only one per submesh: %s", "metallicRoughnessTexture", filename.c_str());
            continue;
        }

        if (!loadGLTFTexture(
            mat.occlusionTexture.index, 
            model,
            mesh.submeshes[i].material.occlusionTexture, 
            filename + "|" + "occlusionTexture"))
            continue;

        if (mat.occlusionTexture.texCoord != 0){
            Log::error("Not supported texcoord for %s, only one per submesh: %s", "occlusionTexture", filename.c_str());
            continue;
        }

        if (!loadGLTFTexture(
            mat.emissiveTexture.index, 
            model,
            mesh.submeshes[i].material.emissiveTexture, 
            filename + "|" + "emissiveTexture"))
            continue;

        if (mat.emissiveTexture.texCoord != 0){
            Log::error("Not supported texcoord for %s, only one per submesh: %s", "emissiveTexture", filename.c_str());
            continue;
        }

        if (!loadGLTFTexture(
            mat.normalTexture.index, 
            model,
            mesh.submeshes[i].material.normalTexture, 
            filename + "|" + "normalTexture"))
            continue;

        if (mat.normalTexture.texCoord != 0){
            Log::error("Not supported texcoord for %s, only one per submesh: %s", "normalTexture", filename.c_str());
            continue;
        }

        mesh.submeshes[i].material.baseColorFactor = Vector4(
            mat.pbrMetallicRoughness.baseColorFactor[0],
            mat.pbrMetallicRoughness.baseColorFactor[1],
            mat.pbrMetallicRoughness.baseColorFactor[2],
            mat.pbrMetallicRoughness.baseColorFactor[3]);

        mesh.submeshes[i].material.metallicFactor = mat.pbrMetallicRoughness.metallicFactor;

        mesh.submeshes[i].material.roughnessFactor = mat.pbrMetallicRoughness.roughnessFactor;

        mesh.submeshes[i].material.emissiveFactor = Vector3(
            mat.emissiveFactor[0],
            mat.emissiveFactor[1],
            mat.emissiveFactor[2]);

        loadGLTFBuffer(indexAccessor.bufferView, mesh, model, 0, loadedBuffers);

        addSubmeshAttribute(mesh.submeshes[i], getBufferName(indexAccessor.bufferView, model), AttributeType::INDEX, 1, indexType, indexAccessor.count, indexAccessor.byteOffset, false);

        for (auto &attrib : primitive.attributes) {
            tinygltf::Accessor accessor = model.gltfModel->accessors[attrib.second];
            int byteStride = accessor.ByteStride(model.gltfModel->bufferViews[accessor.bufferView]);
            std::string bufferName = getBufferName(accessor.bufferView, model);

            loadGLTFBuffer(accessor.bufferView, mesh, model, byteStride, loadedBuffers);

            int elements = 1;
            if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                elements = accessor.type;
            }

            AttributeDataType dataType;

            if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE){
                dataType = AttributeDataType::BYTE;
            }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE){
                dataType = AttributeDataType::UNSIGNED_BYTE;
            }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT){
                dataType = AttributeDataType::SHORT;
            }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT){
                dataType = AttributeDataType::UNSIGNED_SHORT;
            }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT){
                //dataType = AttributeDataType::UNSIGNED_INT;
            }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT){
                dataType = AttributeDataType::FLOAT;
            }else{
                Log::error("Unknown data type %i of %s", accessor.componentType, attrib.first.c_str());
                continue;
            }

            AttributeType attType;
            bool foundAttrs = false;
            if (attrib.first.compare("POSITION") == 0){
                mesh.vertexCount = accessor.count;
                attType = AttributeType::POSITION;
                foundAttrs = true;
            }
            if (attrib.first.compare("NORMAL") == 0){
                attType = AttributeType::NORMAL;
                foundAttrs = true;
            }
            if (attrib.first.compare("TANGENT") == 0){
                attType = AttributeType::TANGENT;
                foundAttrs = true;
            }
            if (attrib.first.compare("TEXCOORD_0") == 0){
                attType = AttributeType::TEXCOORD1;
                foundAttrs = true;
            }
            if (attrib.first.compare("TEXCOORD_1") == 0){
                //attType = AttributeType::TEXTURECOORDS;
                //foundAttrs = true;
            }
            if (attrib.first.compare("COLOR_0") == 0){
                if (elements == 4){
                    attType = AttributeType::COLOR;
                    foundAttrs = true;
                    mesh.submeshes[i].hasVertexColor4 = true;
                } else {
                    Log::warn("Not supported vector(3) of: %s", attrib.first.c_str());
                }
            }
            if (attrib.first.compare("JOINTS_0") == 0){
                attType = AttributeType::BONEIDS;
                foundAttrs = true;

                // Sokol always normalize unsigned short
                if (dataType == AttributeDataType::UNSIGNED_SHORT){
                    mesh.normAdjustJoint = 65535.0;
                }
            }
            if (attrib.first.compare("WEIGHTS_0") == 0){
                attType = AttributeType::BONEWEIGHTS;
                foundAttrs = true;

                if (accessor.normalized){
                    if (dataType == AttributeDataType::BYTE){
                        mesh.normAdjustWeight = 127.0;
                    }else if (dataType == AttributeDataType::UNSIGNED_BYTE){
                        mesh.normAdjustWeight = 255.0;
                    }else if (dataType == AttributeDataType::SHORT){
                        mesh.normAdjustWeight = 32767.0;
                    }
                }
                // Sokol always normalize unsigned short
                if (dataType == AttributeDataType::UNSIGNED_SHORT){
                    mesh.normAdjustWeight = 65535.0;
                }
            }

            if (foundAttrs) {
                for (int i = 0; i < mesh.numExternalBuffers; i++){
                    if (mesh.eBuffers[i].getName() == bufferName)
                        mesh.eBuffers[i].setRenderAttributes(false);
                }
                addSubmeshAttribute(mesh.submeshes[i], bufferName, attType, elements, dataType, accessor.count, accessor.byteOffset, accessor.normalized);
            } else
                Log::warn("Model attribute unused: %s", attrib.first.c_str());

        }

        bool morphTargets = false;
        bool morphNormals = false;
        bool morphTangents = false;

        int morphIndex = 0;
        for (auto &morphs : primitive.targets) {
            for (auto &attribMorph : morphs) {

                morphTargets = true;

                tinygltf::Accessor accessor = model.gltfModel->accessors[attribMorph.second];
                int byteStride = accessor.ByteStride(model.gltfModel->bufferViews[accessor.bufferView]);
                std::string bufferName = getBufferName(accessor.bufferView, model);

                loadGLTFBuffer(accessor.bufferView, mesh, model, byteStride, loadedBuffers);

                int elements = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                    elements = accessor.type;
                }

                AttributeDataType dataType;

                if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE){
                    dataType = AttributeDataType::BYTE;
                }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE){
                    dataType = AttributeDataType::UNSIGNED_BYTE;
                }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT){
                    dataType = AttributeDataType::SHORT;
                }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT){
                    dataType = AttributeDataType::UNSIGNED_SHORT;
                }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT){
                    dataType = AttributeDataType::UNSIGNED_INT;
                }else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT){
                    dataType = AttributeDataType::FLOAT;
                }else{
                    Log::error("Unknown data type %i of morph target %s", accessor.componentType, attribMorph.first.c_str());
                    continue;
                }

                AttributeType attType;
                bool foundMorph = false;

                if (attribMorph.first.compare("POSITION") == 0){
                    if (morphIndex == 0){
                        attType = AttributeType::MORPHTARGET0;
                        foundMorph = true;
                    } else if (morphIndex == 1){
                        attType = AttributeType::MORPHTARGET1;
                        foundMorph = true;
                    }
                    if ((!morphNormals && morphTangents) || (morphNormals && !morphTangents)){
                        if (morphIndex == 2){
                            attType = AttributeType::MORPHTARGET2;
                            foundMorph = true;
                        } else if (morphIndex == 3){
                            attType = AttributeType::MORPHTARGET3;
                            foundMorph = true;
                        }
                        if (!morphNormals && !morphTangents){
                            if (morphIndex == 4){
                                attType = AttributeType::MORPHTARGET4;
                                foundMorph = true;
                            } else if (morphIndex == 5){
                                attType = AttributeType::MORPHTARGET5;
                                foundMorph = true;
                            } else if (morphIndex == 6){
                                attType = AttributeType::MORPHTARGET6;
                                foundMorph = true;
                            } else if (morphIndex == 7){
                                attType = AttributeType::MORPHTARGET7;
                                foundMorph = true;
                            }
                        }
                    }
                }
                if (attribMorph.first.compare("NORMAL") == 0){
                    morphNormals = true;
                    if (morphIndex == 0){
                        attType = AttributeType::MORPHNORMAL0;
                        foundMorph = true;
                    } else if (morphIndex == 1){
                        attType = AttributeType::MORPHNORMAL1;
                        foundMorph = true;
                    }
                    if (!morphTangents){
                        if (morphIndex == 2){
                            attType = AttributeType::MORPHNORMAL2;
                            foundMorph = true;
                        } else if (morphIndex == 3){
                            attType = AttributeType::MORPHNORMAL3;
                            foundMorph = true;
                        }
                    }
                }
                if (attribMorph.first.compare("TANGENT") == 0){
                    morphTangents = true;
                    if (morphIndex == 0){
                        attType = AttributeType::MORPHTANGENT0;
                        foundMorph = true;
                    } else if (morphIndex == 1){
                        attType = AttributeType::MORPHTANGENT1;
                        foundMorph = true;
                    }
                }

                if (foundMorph) {
                    for (int i = 0; i < mesh.numExternalBuffers; i++){
                        if (mesh.eBuffers[i].getName() == bufferName)
                            mesh.eBuffers[i].setRenderAttributes(false);
                    }
                    addSubmeshAttribute(mesh.submeshes[i], bufferName, attType, elements, dataType, accessor.count, accessor.byteOffset, accessor.normalized);
                }
            }
            morphIndex++;
        }

        int morphWeightSize = 8;
        if (morphNormals){
            morphWeightSize = 4;
        }

        if (morphTargets){
            for (int w = 0; w < gltfmesh.weights.size(); w++) {
                if (w < MAX_MORPHTARGETS){
                    mesh.morphWeights[w] = gltfmesh.weights[w];
                }
            }

            //Getting morph target names from mesh extra property
            model.morphNameMapping.clear();
            if (gltfmesh.extras.Has("targetNames") && gltfmesh.extras.Get("targetNames").IsArray()){
                for (int t = 0; t < gltfmesh.extras.Get("targetNames").Size(); t++){
                    model.morphNameMapping[gltfmesh.extras.Get("targetNames").Get(t).Get<std::string>()] = t;
                }
            }
        }

    }

    int skinIndex = model.gltfModel->nodes[meshNode].skin;
    int skeletonRoot = -1;

    if (skinIndex >= 0){
        tinygltf::Skin skin = model.gltfModel->skins[skinIndex];

        if (skin.skeleton >= 0) {
            skeletonRoot = skin.skeleton;
        }else {
            //Find skeleton root
            for (int j = 0; j < skin.joints.size(); j++) {
                int nodeIndex = skin.joints[j];

                if (std::find(skin.joints.begin(), skin.joints.end(), nodesParent[nodeIndex]) == skin.joints.end())
                    skeletonRoot = nodeIndex;
            }
        }

        model.bonesNameMapping.clear();
        model.bonesIdMapping.clear();

        model.skeleton = generateSketetalStructure(entity, model, skeletonRoot, skinIndex);

        if (model.skeleton != NULL_ENTITY) {
            if (skin.joints.size() > MAX_BONES){
                Log::error("Cannot create skinning bigger than %i", MAX_BONES);
                return false;
            }
            scene->addEntityChild(entity, model.skeleton);
        }
    }

    clearAnimations(model);

    for (size_t i = 0; i < model.gltfModel->animations.size(); i++) {
        const tinygltf::Animation &animation = model.gltfModel->animations[i];

        Entity anim;

        anim = scene->createEntity();
        scene->addComponent<ActionComponent>(anim, {});
        scene->addComponent<AnimationComponent>(anim, {});

        AnimationComponent& animcomp = scene->getComponent<AnimationComponent>(anim);

        animcomp.name = animation.name;
        animcomp.ownedActions = true;

        model.animations.push_back(anim);

        for (size_t j = 0; j < animation.channels.size(); j++) {

            const tinygltf::AnimationChannel &channel = animation.channels[j];
            const tinygltf::AnimationSampler &sampler = animation.samplers[channel.sampler];

            tinygltf::Accessor accessorIn = model.gltfModel->accessors[sampler.input];
            tinygltf::BufferView bufferViewIn = model.gltfModel->bufferViews[accessorIn.bufferView];

            tinygltf::Accessor accessorOut = model.gltfModel->accessors[sampler.output];
            tinygltf::BufferView bufferViewOut = model.gltfModel->bufferViews[accessorOut.bufferView];

            //TODO: Implement rotation and weights non float
            if (accessorOut.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {

                if (accessorIn.count != accessorOut.count) {
                    Log::error("Incorrect frame size in animation: %s, sampler: %i",
                               animation.name.c_str(), channel.sampler);
                }

                float *timeValues = (float *) (&model.gltfModel->buffers[bufferViewIn.buffer].data.at(0) +
                                               bufferViewIn.byteOffset + accessorIn.byteOffset);
                float *values = (float *) (&model.gltfModel->buffers[bufferViewOut.buffer].data.at(0) +
                                           bufferViewOut.byteOffset + accessorOut.byteOffset);

                float duration = timeValues[accessorIn.count - 1];

                Entity track;

                track = scene->createEntity();

                scene->addComponent<ActionComponent>(track, {});
                scene->addComponent<KeyframeTracksComponent>(track, {});

                ActionComponent& actiontrack = scene->getComponent<ActionComponent>(track);
                KeyframeTracksComponent& keyframe = scene->getComponent<KeyframeTracksComponent>(track);

                bool foundTrack = false;
                if (channel.target_path.compare("translation") == 0) {
                    foundTrack = true;
                    scene->addComponent<TranslateTracksComponent>(track, {});
                    TranslateTracksComponent& translatetracks = scene->getComponent<TranslateTracksComponent>(track);
                    for (int c = 0; c < accessorIn.count; c++) {
                        Vector3 positionAc(values[3 * c], values[(3 * c) + 1], values[(3 * c) + 2]);

                        keyframe.times.push_back(timeValues[c]);
                        translatetracks.values.push_back(positionAc);
                    }
                }
                if (channel.target_path.compare("rotation") == 0) {
                    foundTrack = true;
                    scene->addComponent<RotateTracksComponent>(track, {});
                    RotateTracksComponent& rotatetracks = scene->getComponent<RotateTracksComponent>(track);
                    for (int c = 0; c < accessorIn.count; c++) {
                        Quaternion rotationAc(values[(4 * c) + 3], values[4 * c], values[(4 * c) + 1], values[(4 * c) + 2]);

                        keyframe.times.push_back(timeValues[c]);
                        rotatetracks.values.push_back(rotationAc);
                    }
                }
                if (channel.target_path.compare("scale") == 0) {
                    foundTrack = true;
                    scene->addComponent<ScaleTracksComponent>(track, {});
                    ScaleTracksComponent& scaletracks = scene->getComponent<ScaleTracksComponent>(track);
                    for (int c = 0; c < accessorIn.count; c++) {
                        Vector3 scaleAc(values[3 * c], values[(3 * c) + 1], values[(3 * c) + 2]);

                        keyframe.times.push_back(timeValues[c]);
                        scaletracks.values.push_back(scaleAc);
                    }
                }
                if (channel.target_path.compare("weights") == 0) {
                    foundTrack = true;
                    scene->addComponent<MorphTracksComponent>(track, {});
                    MorphTracksComponent& morphtracks = scene->getComponent<MorphTracksComponent>(track);
                    int morphNum = accessorOut.count / accessorIn.count;
                    for (int c = 0; c < accessorIn.count; c++) {
                        std::vector<float> weightsAc;
                        for (int m = 0; m < morphNum; m++) {
                            weightsAc.push_back(values[(morphNum * c) + m]);
                        }

                        keyframe.times.push_back(timeValues[c]);
                        morphtracks.values.push_back(weightsAc);
                    }
                }

                if (foundTrack) {
                    if (model.bonesIdMapping.count(channel.target_node)) {
                        actiontrack.target = model.bonesIdMapping[channel.target_node];
                    } else {
                        actiontrack.target = entity;
                    }
                    animcomp.actions.push_back({0, duration, track});
                }else{
                    scene->destroyEntity(track);
                }

            }else{
                Log::error("Cannot load animation: %s, channel %i: no float elements", animation.name.c_str(), j);
            }
        }

        // need to get here because other actions were created
        ActionComponent& anim_actioncomp = scene->getComponent<ActionComponent>(anim);

        anim_actioncomp.target = entity;

    }

/*
    //BEGIN DEBUG
    for (auto &gltfmesh : gltfModel->meshes) {
        Log::verbose("mesh : %s", gltfmesh.name.c_str());
        for (auto &primitive : gltfmesh.primitives) {
            const tinygltf::Accessor &indexAccessor = gltfModel->accessors[primitive.indices];

            Log::verbose("indexaccessor: count %i, type %i", indexAccessor.count,
                    indexAccessor.componentType);

            tinygltf::Material &mat = gltfModel->materials[primitive.material];
            for (auto &mats : mat.values) {
                Log::verbose("mat : %s", mats.first.c_str());
            }

            for (auto &image : gltfModel->images) {
                Log::verbose("image name : %s", image.uri.c_str());
                Log::verbose("  size : %i", image.image.size());
                Log::verbose("  w/h : %i/%i", image.width, image.height);
            }

            Log::verbose("indices : %i", primitive.indices);
            Log::verbose("mode     : %i", primitive.mode);

            for (auto &attrib : primitive.attributes) {
                Log::verbose("attribute : %s", attrib.first.c_str());
            }
        }
    }
    //END DEBUG
*/
    std::reverse(mesh.submeshes, mesh.submeshes + mesh.numSubmeshes);

    return true;
}

bool MeshSystem::loadOBJ(Entity entity, std::string filename){
    MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);
    ModelComponent& model = scene->getComponent<ModelComponent>(entity);
    Transform& transform = scene->getComponent<Transform>(entity);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    tinyobj::FileReader::externalFunc = readFileToString;

    std::string baseDir = FileData::getBaseDir(filename);

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), baseDir.c_str());

    if (!warn.empty()) {
        Log::warn("Loading OBJ model (%s): %s", filename.c_str(), warn.c_str());
    }

    if (!err.empty()) {
        Log::error("Can't load OBJ model (%s): %s", filename.c_str(), err.c_str());
        return false;
    }

    if (ret) {

        mesh.buffer.clear();
        mesh.buffer.addAttribute(AttributeType::POSITION, 3);
        mesh.buffer.addAttribute(AttributeType::TEXCOORD1, 2);
        mesh.buffer.addAttribute(AttributeType::NORMAL, 3);
        mesh.buffer.addAttribute(AttributeType::COLOR, 4);

        mesh.indices.clear();

        mesh.numSubmeshes = materials.size();

        for (size_t i = 0; i < materials.size(); i++) {

            mesh.submeshes[i].attributes.clear();

            // Convert the blinn-phong model to the pbr metallic-roughness model
            // Based on https://github.com/CesiumGS/obj2gltf
            const float specularIntensity = materials[i].specular[0] * 0.2125 + materials[i].specular[1] * 0.7154 + materials[i].specular[2] * 0.0721; //luminance

            float roughnessFactor = materials[i].shininess;
            roughnessFactor = roughnessFactor / 1000.0;
            roughnessFactor = 1.0 - roughnessFactor;
            roughnessFactor = std::min(std::max(roughnessFactor, 0.0f), 1.0f); //clamp

            if (specularIntensity < 0.1) {
                roughnessFactor *= (1.0 - specularIntensity);
            }

            const float metallicFactor = 0.0;

            materials[i].specular[0] = metallicFactor;
            materials[i].specular[1] = metallicFactor;
            materials[i].specular[2] = metallicFactor;

            materials[i].shininess = roughnessFactor;
            // ------ End convertion

            mesh.submeshes[i].material.baseColorFactor = Vector4(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2], 1.0);
            mesh.submeshes[i].material.emissiveFactor = Vector3(materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
            mesh.submeshes[i].material.metallicFactor = materials[i].specular[0];
            mesh.submeshes[i].material.roughnessFactor = materials[i].shininess;

            if (!materials[i].diffuse_texname.empty())
                mesh.submeshes[i].material.baseColorTexture.setPath(baseDir+materials[i].diffuse_texname);
            if (!materials[i].normal_texname.empty())
                mesh.submeshes[i].material.normalTexture.setPath(baseDir+materials[i].normal_texname);
            if (!materials[i].emissive_texname.empty())
                mesh.submeshes[i].material.emissiveTexture.setPath(baseDir+materials[i].emissive_texname);
            if (!materials[i].ambient_texname.empty())
                mesh.submeshes[i].material.occlusionTexture.setPath(baseDir+materials[i].ambient_texname);

            //TODO: occlusionFactor (Ka)
            //TODO: metallicroughnessTexture (map_Ks + map_Ns)

            if (materials[i].dissolve < 1){
                mesh.transparent = true;
            }
        }

        Attribute* attVertex = mesh.buffer.getAttribute(AttributeType::POSITION);
        Attribute* attTexcoord = mesh.buffer.getAttribute(AttributeType::TEXCOORD1);
        Attribute* attNormal = mesh.buffer.getAttribute(AttributeType::NORMAL);
        Attribute* attColor = mesh.buffer.getAttribute(AttributeType::COLOR);

        std::vector<std::vector<uint16_t>> indexMap;
        if (materials.size() > 0) {
            indexMap.resize(materials.size());
        }else{
            indexMap.resize(1);
        }

        for (size_t i = 0; i < shapes.size(); i++) {

            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
                size_t fnum = shapes[i].mesh.num_face_vertices[f];

                int material_id = shapes[i].mesh.material_ids[f];
                if (material_id < 0)
                    material_id = 0;

                // For each vertex in the face
                for (size_t v = 0; v < fnum; v++) {
                    tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];

                    indexMap[material_id].push_back(mesh.buffer.getCount());

                     mesh.buffer.addVector3(attVertex,
                                          Vector3(attrib.vertices[3*idx.vertex_index+0],
                                                  attrib.vertices[3*idx.vertex_index+1],
                                                  attrib.vertices[3*idx.vertex_index+2]));

                    if (attrib.texcoords.size() > 0) {
                         mesh.buffer.addVector2(attTexcoord,
                                              Vector2(attrib.texcoords[2 * idx.texcoord_index + 0],
                                                      1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]));
                    }
                    if (attrib.normals.size() > 0) {
                         mesh.buffer.addVector3(attNormal,
                                              Vector3(attrib.normals[3 * idx.normal_index + 0],
                                                      attrib.normals[3 * idx.normal_index + 1],
                                                      attrib.normals[3 * idx.normal_index + 2]));
                    }

                    if (attrib.colors.size() > 0){
                         mesh.buffer.addVector4(attColor,
                                              Vector4(attrib.colors[3 * idx.vertex_index + 0],
                                                      attrib.colors[3 * idx.vertex_index + 1],
                                                      attrib.colors[3 * idx.vertex_index + 2],
                                                      1.0));
                    }else{
                         mesh.buffer.addVector4(attColor, Vector4(1.0, 1.0, 1.0, 1.0));
                    }

                }

                index_offset += fnum;
            }
        }

        for (size_t i = 0; i < mesh.numSubmeshes; i++) {
            addSubmeshAttribute(mesh.submeshes[i], "indices", AttributeType::INDEX, 1, AttributeDataType::UNSIGNED_SHORT, indexMap[i].size(), mesh.indices.getCount() * sizeof(uint16_t), false);

            mesh.indices.setValues(mesh.indices.getCount(),  mesh.indices.getAttribute(AttributeType::INDEX), indexMap[i].size(), (char*)&indexMap[i].front(), sizeof(uint16_t));
            mesh.indices.setRenderAttributes(false);
        }

        std::reverse(mesh.submeshes, mesh.submeshes + mesh.numSubmeshes);
    }

    return true;
}

void MeshSystem::destroyModel(ModelComponent& model){
    if (model.gltfModel){
        delete model.gltfModel;
        model.gltfModel = NULL;
    }

    for (auto const& bone : model.bonesIdMapping){
        scene->destroyEntity(bone.second);
    }
    model.bonesIdMapping.clear();
    model.bonesNameMapping.clear();

    clearAnimations(model);

    model.morphNameMapping.clear();

    model.skeleton = NULL_ENTITY;
}

bool MeshSystem::createOrUpdateSprite(SpriteComponent& sprite, MeshComponent& mesh){
    if (sprite.needUpdateSprite){
        CameraComponent& camera =  scene->getComponent<CameraComponent>(scene->getCamera());
        if (sprite.automaticFlipY){
            changeFlipY(sprite.flipY, camera, mesh);
        }

        createSprite(sprite, mesh, camera);

        sprite.needUpdateSprite = false;
    }

    return true;
}

bool MeshSystem::createOrUpdateTerrain(TerrainComponent& terrain){
    if (terrain.needUpdateTerrain){
        createTerrain(terrain);

        terrain.needUpdateTerrain = false;
    }

    return true;
}

bool MeshSystem::createOrUpdateMeshPolygon(MeshPolygonComponent& polygon, MeshComponent& mesh){
    if (polygon.needUpdatePolygon){
        if (polygon.automaticFlipY){
            CameraComponent& camera =  scene->getComponent<CameraComponent>(scene->getCamera());
            changeFlipY(polygon.flipY, camera, mesh);
        }

        createMeshPolygon(polygon, mesh);

        polygon.needUpdatePolygon = false;
    }

    return true;
}

bool MeshSystem::createOrUpdateTilemap(TilemapComponent& tilemap, MeshComponent& mesh){
    if (tilemap.needUpdateTilemap){
        if (tilemap.automaticFlipY){
            CameraComponent& camera =  scene->getComponent<CameraComponent>(scene->getCamera());
            changeFlipY(tilemap.flipY, camera, mesh);
        }

        createTilemap(tilemap, mesh);

        tilemap.needUpdateTilemap = false;
    }

    return true;
}

void MeshSystem::load(){
    update(0);
}

void MeshSystem::destroy(){

}

void MeshSystem::update(double dt){

    auto sprites = scene->getComponentArray<SpriteComponent>();
    for (int i = 0; i < sprites->size(); i++){
		SpriteComponent& sprite = sprites->getComponentFromIndex(i);

        Entity entity = sprites->getEntity(i);
        Signature signature = scene->getSignature(entity);

        if (signature.test(scene->getComponentType<MeshComponent>())){
            MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

            createOrUpdateSprite(sprite, mesh);
        }
    }

    auto tilemaps = scene->getComponentArray<TilemapComponent>();
    for (int i = 0; i < tilemaps->size(); i++){
		TilemapComponent& tilemap = tilemaps->getComponentFromIndex(i);

        Entity entity = tilemaps->getEntity(i);
        Signature signature = scene->getSignature(entity);

        if (signature.test(scene->getComponentType<MeshComponent>())){
            MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

            createOrUpdateTilemap(tilemap, mesh);
        }
    }

    auto polygons = scene->getComponentArray<MeshPolygonComponent>();
    for (int i = 0; i < polygons->size(); i++){
		MeshPolygonComponent& polygon = polygons->getComponentFromIndex(i);

        Entity entity = polygons->getEntity(i);
        Signature signature = scene->getSignature(entity);

        if (signature.test(scene->getComponentType<MeshComponent>())){
            MeshComponent& mesh = scene->getComponent<MeshComponent>(entity);

            createOrUpdateMeshPolygon(polygon, mesh);
        }
    }

    auto terrains = scene->getComponentArray<TerrainComponent>();
    for (int i = 0; i < terrains->size(); i++){
		TerrainComponent& terrain = terrains->getComponentFromIndex(i);

        createOrUpdateTerrain(terrain);
    }

}

void MeshSystem::draw(){

}

void MeshSystem::entityDestroyed(Entity entity){
	Signature signature = scene->getSignature(entity);

	if (signature.test(scene->getComponentType<ModelComponent>())){
        destroyModel(scene->getComponent<ModelComponent>(entity));
	}
}