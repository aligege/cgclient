//
// (c) 2022 Eduardo Doria.
//

#ifndef SCALETRACKS_H
#define SCALETRACKS_H

#include "action/Action.h"

namespace Supernova{
    class ScaleTracks: public Action{

    public:
        ScaleTracks(Scene* scene);
        ScaleTracks(Scene* scene, std::vector<float> times, std::vector<Vector3> values);

        void setTimes(std::vector<float> times);
        void setValues(std::vector<Vector3> values);
    };
}

#endif //SCALETRACKS_H