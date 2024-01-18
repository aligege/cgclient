//
// (c) 2023 Eduardo Doria.
//

#ifndef JOINT2D_H
#define JOINT2D_H

#include "EntityHandle.h"

namespace cg{

    class Joint2D: public EntityHandle{
    public:
        Joint2D(Scene* scene);
        Joint2D(Scene* scene, Entity entity);
        virtual ~Joint2D();

        Joint2D(const Joint2D& rhs);
        Joint2D& operator=(const Joint2D& rhs);

        b2Joint* getBox2DJoint() const;

        void setDistanceJoint(Entity bodyA, Entity bodyB, Vector2 worldAnchorOnBodyA, Vector2 worldAnchorOnBodyB);    
        void setRevoluteJoint(Entity bodyA, Entity bodyB, Vector2 worldAnchor);
        void setPrismaticJoint(Entity bodyA, Entity bodyB, Vector2 worldAnchor, Vector2 worldAxis);
        void setPulleyJoint(Entity bodyA, Entity bodyB, Vector2 groundAnchorA, Vector2 groundAnchorB, Vector2 worldAnchorOnBodyA, Vector2 worldAnchorOnBodyB, Vector2 worldAxis, float ratio);
        void setGearJoint(Entity bodyA, Entity bodyB, Entity revoluteJoint, Entity prismaticJoint, float ratio);
        void setMouseJoint(Entity body, Vector2 target);
        void setWheelJoint(Entity bodyA, Entity bodyB, Vector2 worldAnchor, Vector2 worldAxis);
        void setWeldJoint(Entity bodyA, Entity bodyB, Vector2 worldAnchor);
        void setFrictionJoint(Entity bodyA, Entity bodyB, Vector2 worldAnchor);
        void setMotorJoint(Entity bodyA, Entity bodyB);
        void setRopeJoint(Entity bodyA, Entity bodyB, Vector2 worldAnchorOnBodyA, Vector2 worldAnchorOnBodyB);

        Joint2DType getType();
    };
}

#endif //BODY2D_H