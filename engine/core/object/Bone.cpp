//
// (c) 2022 Eduardo Doria.
//

#include "Bone.h"


using namespace cg;

Bone::Bone(Scene* scene, Entity entity): Object(scene, entity){
    
}

Bone::~Bone(){

}

Bone::Bone(const Bone& rhs): Object(rhs){
    
}

Bone& Bone::operator=(const Bone& rhs){
    Object::operator =(rhs);

    return *this;
}

