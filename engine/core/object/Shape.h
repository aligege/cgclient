//
// (c) 2023 Eduardo Doria.
//

#ifndef SHAPE_H
#define SHAPE_H

#include "Mesh.h"

namespace Supernova{

    class Shape: public Mesh{
    public:
        Shape(Scene* scene);
        virtual ~Shape();

        void createPlane(float width, float depth);
        void createPlane(float width, float depth, unsigned int tiles);
        
        void createBox(float width, float height, float depth);
        void createBox(float width, float height, float depth, unsigned int tiles);

        void createSphere(float radius);
        void createSphere(float radius, unsigned int slices, unsigned int stacks);

        void createCylinder(float radius, float height);
        void createCylinder(float baseRadius, float topRadius, float height);
        void createCylinder(float radius, float height, unsigned int slices, unsigned int stacks);
        void createCylinder(float baseRadius, float topRadius, float height, unsigned int slices, unsigned int stacks);

        void createTorus(float radius, float ringRadius);
        void createTorus(float radius, float ringRadius, unsigned int sides, unsigned int rings);
    };
}

#endif //SHAPE_H