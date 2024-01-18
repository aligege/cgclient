//
// (c) 2022 Eduardo Doria.
//

#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include "buffer/ExternalBuffer.h"
#include <vector>
#include <map>

namespace tinygltf {class Model;}

namespace cg{

    struct ModelComponent{
        tinygltf::Model* gltfModel = NULL;

        Matrix4 inverseDerivedTransform;
        
        Entity skeleton;

        std::map<std::string, Entity> bonesNameMapping;
        std::map<int, Entity> bonesIdMapping;

        std::map<std::string, int> morphNameMapping;

        std::vector<Entity> animations;
    };

}

#endif //MODEL_COMPONENT_H