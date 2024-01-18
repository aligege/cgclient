//
// (c) 2021 Eduardo Doria.
//

#ifndef COLORACTION_H
#define COLORACTION_H

#include "TimedAction.h"

namespace cg{
    class ColorAction: public TimedAction{

    public:
        ColorAction(Scene* scene);

        void setAction(Vector3 startColor, Vector3 endColor, float duration, bool loop=false);
        void setAction(Vector4 startColor, Vector4 endColor, float duration, bool loop=false);
    };
}

#endif //COLORACTION_H