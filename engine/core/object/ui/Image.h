//
// (c) 2022 Eduardo Doria.
//

#ifndef IMAGE_H
#define IMAGE_H

#include "UILayout.h"

namespace Supernova{
    class Image: public UILayout{
    public:
        Image(Scene* scene);

        bool createImage();
        bool load();

        void setPatchMargin(int margin);
        void setPatchMargin(int marginLeft, int marginRight, int marginTop, int marginBottom);

        void setPatchMarginBottom(int marginBottom);
        void setPatchMarginLeft(int marginLeft);
        void setPatchMarginRight(int marginRight);
        void setPatchMarginTop(int marginTop);

        int getPatchMarginBottom() const;
        int getPatchMarginLeft() const;
        int getPatchMarginRight() const;
        int getPatchMarginTop() const;

        void setTexture(std::string path);
        void setTexture(Framebuffer* framebuffer);

        void setTextureCutFactor(float textureCutFactor);
        float getTextureCutFactor() const;

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

#endif //IMAGE_H