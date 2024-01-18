#ifndef ROTATETRACKS_COMPONENT_H
#define ROTATETRACKS_COMPONENT_H

#include "Engine.h"

namespace cg{

    struct RotateTracksComponent{
        std::vector<Quaternion> values;
    };

}

#endif //ROTATETRACKS_COMPONENT_H