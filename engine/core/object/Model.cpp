//
// (c) 2021 Eduardo Doria.
//

#include "Model.h"

#include "io/FileData.h"
#include "io/Data.h"
#include "texture/TextureData.h"
#include "pool/TexturePool.h"

#include "subsystem/MeshSystem.h"
#include "component/ActionComponent.h"
#include "component/AnimationComponent.h"

#include <algorithm>
#include <sstream>
#include <cfloat>

using namespace Supernova;

Model::Model(Scene* scene): Mesh(scene){
    addComponent<ModelComponent>({});

}

Model::~Model(){

}

bool Model::loadModel(std::string filename){
    std::string ext = FileData::getFilePathExtension(filename);

    if (ext.compare("obj") == 0) {
        if (!loadOBJ(filename))
            return false;
    }else{
        if (!loadGLTF(filename))
            return false;
    }

    return true;
}

bool Model::loadOBJ(std::string filename){
    MeshComponent& mesh = getComponent<MeshComponent>();

    bool ret = scene->getSystem<MeshSystem>()->loadOBJ(entity, filename);

    if (ret){
        mesh.needReload = true;
    }

    return ret;
}

bool Model::loadGLTF(std::string filename){
    MeshComponent& mesh = getComponent<MeshComponent>();

    bool ret = scene->getSystem<MeshSystem>()->loadGLTF(entity, filename);

    if (ret){
        mesh.needReload = true;
    }

    return ret;
}

Animation Model::getAnimation(int index){
    ModelComponent& model = getComponent<ModelComponent>();

    try{
        return Animation(scene, model.animations.at(index));
    }catch (const std::out_of_range& e){
		Log::error("Retrieving non-existent animation: %s", e.what());
		throw;
	}
}

Animation Model::findAnimation(std::string name){
    ModelComponent& model = getComponent<ModelComponent>();

    for (int i = 0; i < model.animations.size(); i++){
        Signature signature = scene->getSignature(model.animations[i]);
        if (signature.test(scene->getComponentType<AnimationComponent>())){
            AnimationComponent& animcomp = scene->getComponent<AnimationComponent>(model.animations[i]);

            if (animcomp.name == name){
                return Animation(scene, model.animations[i]);
            }
        }
    }
    Log::error("Retrieving non-existent bone: %s", name.c_str());
    throw std::out_of_range("vector animations is out of range");
}

Bone Model::getBone(std::string name){
    ModelComponent& model = getComponent<ModelComponent>();

    try{
        return Bone(scene, model.bonesNameMapping.at(name));
    }catch (const std::out_of_range& e){
		Log::error("Retrieving non-existent bone: %s", e.what());
		throw;
	}
}

Bone Model::getBone(int id){
    ModelComponent& model = getComponent<ModelComponent>();

    try{
        return Bone(scene, model.bonesIdMapping.at(id));
    }catch (const std::out_of_range& e){
		Log::error("Retrieving non-existent bone: %s", e.what());
		throw;
	}
}

float Model::getMorphWeight(std::string name){
    ModelComponent& model = getComponent<ModelComponent>();

    if (model.morphNameMapping.count(name)){
        return getMorphWeight(model.morphNameMapping.at(name));
    }else{
        Log::error("Retrieving non-existent morph weight '%s'", name.c_str());
    }

    return 0;
}

float Model::getMorphWeight(int id){
    MeshComponent& mesh = getComponent<MeshComponent>();

    if (id >= 0 && id < MAX_MORPHTARGETS){
        return mesh.morphWeights[id];
    }else{
        Log::error("Retrieving non-existent morph weight '%i'", id);
    }

    return 0;
}

void Model::setMorphWeight(std::string name, float value){
    ModelComponent& model = getComponent<ModelComponent>();

    if (model.morphNameMapping.count(name)){
        setMorphWeight(model.morphNameMapping.at(name), value);
    }else{
        Log::error("Retrieving non-existent morph weight '%s'", name.c_str());
    }
}

void Model::setMorphWeight(int id, float value){
    MeshComponent& mesh = getComponent<MeshComponent>();

    if (id >= 0 && id < MAX_MORPHTARGETS){
        mesh.morphWeights[id] = value;
    }else{
        Log::error("Retrieving non-existent morph weight '%i'", id);
    }
}