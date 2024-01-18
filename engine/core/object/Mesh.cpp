//
// (c) 2023 Eduardo Doria.
//

#include "Mesh.h"
#include "render/ObjectRender.h"
#include "util/Color.h"
#include "subsystem/RenderSystem.h"

using namespace Supernova;

Mesh::Mesh(Scene* scene): Object(scene){
    addComponent<MeshComponent>({});
}

Mesh::~Mesh(){
}

bool Mesh::load(){
    MeshComponent& mesh = getComponent<MeshComponent>();

    return scene->getSystem<RenderSystem>()->loadMesh(entity, mesh);
}

void Mesh::setTexture(std::string path){
    MeshComponent& mesh = getComponent<MeshComponent>();

    mesh.submeshes[0].material.baseColorTexture.setPath(path);

    mesh.submeshes[0].needUpdateTexture = true;
}

void Mesh::setTexture(Framebuffer* framebuffer){
    MeshComponent& mesh = getComponent<MeshComponent>();

    mesh.submeshes[0].material.baseColorTexture.setFramebuffer(framebuffer);

    mesh.submeshes[0].needUpdateTexture = true;
}

void Mesh::setColor(Vector4 color){
    MeshComponent& mesh = getComponent<MeshComponent>();

    mesh.submeshes[0].material.baseColorFactor = Color::sRGBToLinear(color);
}

void Mesh::setColor(const float red, const float green, const float blue, const float alpha){
    setColor(Vector4(red, green, blue, alpha));
}

void Mesh::setColor(const float red, const float green, const float blue){
    setColor(Vector4(red, green, blue, getColor().w));
}

void Mesh::setAlpha(const float alpha){
    Vector4 color = getColor();
    setColor(Vector4(color.x, color.y, color.z, alpha));
}

Vector4 Mesh::getColor() const{
    MeshComponent& mesh = getComponent<MeshComponent>();

    return Color::linearTosRGB(mesh.submeshes[0].material.baseColorFactor);
}

float Mesh::getAlpha() const{
    return getColor().w;
}

Material& Mesh::getMaterial(unsigned int submesh){
    MeshComponent& mesh = getComponent<MeshComponent>();

    return mesh.submeshes[submesh].material;
}
