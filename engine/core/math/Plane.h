#ifndef Plane_h
#define Plane_h

#include "Vector3.h"
#include "AlignedBox.h"

namespace Supernova{
    
    class Plane{

    public:

        enum Side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE,
            BOTH_SIDE
        };

        Vector3 normal;
        float d;

        Plane ();
        Plane (const Plane& rhs);
        Plane (const Vector3& rkNormal, float fConstant);
        Plane (float a, float b, float c, float d);
        Plane (const Vector3& rkNormal, const Vector3& rkPoint);
        Plane (const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2);

        Plane operator - () const;
        bool operator==(const Plane& rhs) const;
        bool operator!=(const Plane& rhs) const;

        Side getSide (const Vector3& rkPoint) const;
        Side getSide (const Vector3& centre, const Vector3& halfSize) const;
        Side getSide (const AlignedBox& rkBox) const;

        float getDistance (const Vector3& rkPoint) const;

        void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2);
        void redefine(const Vector3& rkNormal, const Vector3& rkPoint);

        Vector3 projectVector(const Vector3& v) const;

        float normalize(void);
    
    };
    
}


#endif /* Plane_h */
