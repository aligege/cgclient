//
// (c) 2022 Eduardo Doria.
//

#ifndef FOG_H
#define FOG_H

#include "object/EntityHandle.h"
#include "component/FogComponent.h"

namespace cg{

    class Fog: public EntityHandle {

    public:
        Fog(Scene* scene);

        FogType getType() const;
        Vector3 getColor() const;
        float getDensity() const;
        float getLinearStart() const;
        float getLinearEnd() const;

        void setType(FogType type);
        void setColor(Vector3 color);
        void setColor(float red, float green, float blue);
        void setDensity(float density);
        void setLinearStart(float start);
        void setLinearEnd(float end);
        void setLinearStartEnd(float start, float end);
    };
}

#endif //FOG_H