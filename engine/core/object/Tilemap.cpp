//
// (c) 2023 Eduardo Doria.
//

#include "Tilemap.h"
#include "subsystem/MeshSystem.h"

using namespace cg;

Tilemap::Tilemap(Scene* scene): Mesh(scene){
    addComponent<TilemapComponent>({});
}

Tilemap::~Tilemap(){

}

bool Tilemap::createTilemap(){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();
    MeshComponent& mesh = getComponent<MeshComponent>();

    return scene->getSystem<MeshSystem>()->createOrUpdateTilemap(tilemap, mesh);
}

int Tilemap::findRectByString(std::string name){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    for (int i = 0; i < MAX_TILEMAP_TILESRECT; i++){
        if (tilemap.tilesRect[i].name == name){
            return i;
        }
    }

    return -1;
}

int Tilemap::findTileByString(std::string name){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    for (int i = 0; i < MAX_TILEMAP_TILES; i++){
        if (tilemap.tiles[i].name == name){
            return i;
        }
    }

    return -1;
}

void Tilemap::setTextureCutFactor(float textureCutFactor){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();
    
    if (tilemap.textureCutFactor != textureCutFactor){
        tilemap.textureCutFactor = textureCutFactor;
        tilemap.needUpdateTilemap = true;
    }
}

float Tilemap::getTextureCutFactor() const{
    TilemapComponent& tilemap = getComponent<TilemapComponent>();
    
    return tilemap.textureCutFactor;
}

void Tilemap::addRect(int id, std::string name, std::string texture, TextureFilter texFilter, Rect rect){
    MeshComponent& mesh = getComponent<MeshComponent>();
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    int submeshId = 0;

    if (id < 0){
        Log::error("Error adding rect with id %i", id);
        return;
    }

    if (id >= MAX_TILEMAP_TILESRECT){
        Log::error("Cannot add rect with id %i, increase MAX_TILEMAP_TILESRECT to bigger than %i", id, id);
        return;
    }

    if (mesh.numSubmeshes >= MAX_SUBMESHES){
        Log::error("Cannot add submesh for rect id %i, increase MAX_SUBMESHES to bigger than %i", id, mesh.numSubmeshes);
        return;
    }

    if (!texture.empty()) {
        bool textureFound = false;
        for (int s = 0; s < mesh.numSubmeshes; s++){
            if (texture == mesh.submeshes[s].material.baseColorTexture.getPath()) {
                textureFound = true;
                submeshId = s;
                break;
            }
        }

        if (!textureFound) {
            submeshId = mesh.numSubmeshes;
            mesh.submeshes[submeshId].material.baseColorTexture.setPath(texture);
            mesh.submeshes[submeshId].material.baseColorTexture.setMinFilter(texFilter);
            mesh.submeshes[submeshId].material.baseColorTexture.setMagFilter(texFilter);
            mesh.submeshes[submeshId].needUpdateTexture = true;
            mesh.numSubmeshes++;
        }
    }

    tilemap.tilesRect[id] = {name, submeshId, rect};
}

void Tilemap::addRect(int id, std::string name, std::string texture, Rect rect){
    addRect(id, name, texture, TextureFilter::LINEAR, rect);
}

void Tilemap::addRect(int id, std::string name, Rect rect){
    addRect(id, name, "", rect);
}

void Tilemap::addRect(std::string name, float x, float y, float width, float height){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    int id = 0;
    for (int i = 0; i < MAX_TILEMAP_TILESRECT; i++){
        if (tilemap.tilesRect[i].submeshId < 0 && tilemap.tilesRect[i].name.empty()){
            id = i;
            break;
        }
    }
    addRect(id, name, Rect(x, y, width, height));
}

void Tilemap::addRect(float x, float y, float width, float height){
    addRect("", x, y, width, height);
}

void Tilemap::addRect(Rect rect){
    addRect(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
}

void Tilemap::removeRect(int id){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    if (id >= 0 && id < MAX_TILEMAP_TILESRECT){
        tilemap.tilesRect[id] = {0};
    }else{
        Log::error("Error removing rect with id %i", id);
    }
}

void Tilemap::removeRect(std::string name){
    int rect = findRectByString(name);
    if (rect >= 0)
        removeRect(rect);
}

void Tilemap::clearRects(){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    for (int i = 0; i < MAX_TILEMAP_TILESRECT; i++){
        tilemap.tilesRect[i].submeshId = -1;
        tilemap.tilesRect[i].name.clear();
    }
}

TileRectData& Tilemap::getRect(int id){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    if (id >= 0 && id < MAX_TILEMAP_TILESRECT){
        return tilemap.tilesRect[id];
    }else{
        throw std::out_of_range("error getting rect tilemap");
    }
}

TileRectData& Tilemap::getRect(std::string name){
    int rect = findRectByString(name);
    if (rect >= 0)
        return getRect(rect);

    throw std::out_of_range("error getting rect tilemap");
}

void Tilemap::addTile(int id, std::string name, int rectId, Vector2 position, float width, float height){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    if (id >= 0 && id < MAX_TILEMAP_TILES){
        tilemap.tiles[id] = {name, rectId, position, width, height};
    }else{
        Log::error("Error adding tile with id %i", id);
    }

    tilemap.needUpdateTilemap = true;
}

void Tilemap::addTile(std::string name, int rectId, Vector2 position, float width, float height){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    int id = 0;
    for (int i = 0; i < MAX_TILEMAP_TILES; i++){
        if (tilemap.tiles[i].width == 0 && tilemap.tiles[i].height == 0 && tilemap.tiles[i].name.empty()){
            id = i;
            break;
        }
    }
    addTile(id, name, rectId, position, width, height);
}

void Tilemap::addTile(int rectId, Vector2 position, float width, float height){
    addTile("", rectId, position, width, height);
}

void Tilemap::addTile(std::string name, std::string rectString, Vector2 position, float width, float height){
    addTile(name, findRectByString(rectString), position, width, height);
}

void Tilemap::addTile(std::string rectString, Vector2 position, float width, float height){
    addTile("", findRectByString(rectString), position, width, height);
}

void Tilemap::removeTile(int id){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    if (id >= 0 && id < MAX_TILEMAP_TILES){
        tilemap.tiles[id] = {};
    }else{
        Log::error("Error removing tile with id %i", id);
    }

    tilemap.needUpdateTilemap = true;
}

void Tilemap::removeTile(std::string name){
    int tile = findTileByString(name);
    if (tile >= 0)
        removeTile(tile);
}

void Tilemap::clearTiles(){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    for (int i = 0; i < MAX_TILEMAP_TILES; i++){
        tilemap.tiles[i].width = 0;
        tilemap.tiles[i].height = 0;
        tilemap.tiles[i].name.clear();
    }
}

TileData& Tilemap::getTile(int id){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    if (id >= 0 && id < MAX_TILEMAP_TILES){
        return tilemap.tiles[id];
    }else{
        throw std::out_of_range("error getting tile");
    }
}

TileData& Tilemap::getTile(std::string name){
    int tile = findTileByString(name);
    if (tile >= 0)
        return getTile(tile);

    throw std::out_of_range("error getting tile");
}

void Tilemap::setReserveTiles(unsigned int reserveTiles){
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    tilemap.reserveTiles = reserveTiles;
}

unsigned int Tilemap::getReserveTiles() const{
    TilemapComponent& tilemap = getComponent<TilemapComponent>();

    return tilemap.reserveTiles;
}

void Tilemap::clearAll(){
    clearTiles();
    clearRects();
}