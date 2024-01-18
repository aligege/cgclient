//
// (c) 2022 Eduardo Doria.
//

#ifndef MORPHTRACKS_H
#define MORPHTRACKS_H

#include "action/Action.h"

namespace cg{
    class MorphTracks: public Action{

    public:
        MorphTracks(Scene* scene);
        MorphTracks(Scene* scene, std::vector<float> times, std::vector<std::vector<float>> values);

        void setTimes(std::vector<float> times);
        void setValues(std::vector<std::vector<float>> values);
    };
}

#endif //MORPHTRACKS_H