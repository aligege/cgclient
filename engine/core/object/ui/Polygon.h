//
// (c) 2022 Eduardo Doria.
//

#ifndef POLYGON_H
#define POLYGON_H

#include "UILayout.h"

namespace cg{
    class Polygon: public UILayout{

    public:
        Polygon(Scene* scene);

        bool createPolygon();
        bool load();

        void addVertex(Vector3 vertex);
        void addVertex(float x, float y);

        void clearVertices();

        void setTexture(std::string path);
        void setTexture(Framebuffer* framebuffer);

        void setColor(Vector4 color);
        void setColor(const float red, const float green, const float blue, const float alpha);
        void setColor(const float red, const float green, const float blue);
        void setAlpha(const float alpha);
        Vector4 getColor() const;
        float getAlpha() const;

        void setFlipY(bool flipY);
        bool isFlipY() const;
    };
}

#endif //POLYGON_H