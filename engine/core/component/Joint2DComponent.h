#ifndef JOINT2D_COMPONENT_H
#define JOINT2D_COMPONENT_H

#include "Engine.h"

class b2JointDef;
class b2Joint;

namespace cg{

    enum class Joint2DType{
        DISTANCE,
        REVOLUTE,
        PRISMATIC,
        PULLEY,
        GEAR,
        MOUSE,
        WHEEL,
        WELD,
        FRICTION,
        MOTOR,
        ROPE // distance in Box2D
    };

    struct Joint2DComponent{
        b2Joint* joint = NULL;
        Joint2DType type = Joint2DType::DISTANCE;
    };

}

#endif //JOINT2D_COMPONENT_H