//
// (c) 2022 Eduardo Doria.
//

#ifndef MESHPOLYGON_H
#define MESHPOLYGON_H

#include "Mesh.h"

namespace cg{
    class MeshPolygon: public Mesh{

    public:
        MeshPolygon(Scene* scene);

        bool createPolygon();

        void addVertex(Vector3 vertex);
        void addVertex(float x, float y);

        void clearVertices();

        int getWidth();
        int getHeight();

        void setFlipY(bool flipY);
        bool isFlipY() const;
    };
}

#endif //MESHPOLYGON_H