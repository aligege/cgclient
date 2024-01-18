//
// (c) 2022 Eduardo Doria.
//

#ifndef ROTATETRACKS_H
#define ROTATETRACKS_H

#include "action/Action.h"

namespace Supernova{
    class RotateTracks: public Action{

    public:
        RotateTracks(Scene* scene);
        RotateTracks(Scene* scene, std::vector<float> times, std::vector<Quaternion> values);

        void setTimes(std::vector<float> times);
        void setValues(std::vector<Quaternion> values);
    };
}

#endif //ROTATETRACKS_H