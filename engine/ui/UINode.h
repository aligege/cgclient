#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "Node.h"
#include "math/Rect.h"
#include "math/Vector2.h"

namespace cg
{
    class UINode: public Node
    {
        protected:
            float _width;
            float _height;
            Vector2 _anchorPoints;
        public:
            void setSize(int width, int height);
            void setWidth(int width);
            void setHeight(int height);

            int getWidth() const;
            int getHeight() const;

            Vector2 getAnchorPoints() const;
            void setAnchorPoints(const Vector2& anchorPoints);
            void setAnchorPoints(float x, float y);
            void setAnchorPointX(float x);
            void setAnchorPointY(float y);
            float getAnchorPointX() const;
            float getAnchorPointY() const;

            void setPosition(const Vector2& position);
            void setPosition(float x, float y);
            Vector2 getPosition() const;
            float getPositionX() const;
            float getPositionY() const;
    };
}

#endif //UI_COMPONENT_H