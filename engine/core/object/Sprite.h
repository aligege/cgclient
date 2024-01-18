//
// (c) 2023 Eduardo Doria.
//

#ifndef SPRITE_H
#define SPRITE_H

#include "buffer/InterleavedBuffer.h"
#include "buffer/IndexBuffer.h"
#include "action/SpriteAnimation.h"

#include "Mesh.h"

namespace cg{
    class Sprite: public Mesh{

    private:
        SpriteAnimation* animation;

    public:
        Sprite(Scene* scene);
        virtual ~Sprite();

        Sprite(const Sprite& rhs);
        Sprite& operator=(const Sprite& rhs);

        bool createSprite();

        void setSize(int width, int height);
        void setWidth(int width);
        void setHeight(int height);

        int getWidth() const;
        int getHeight() const;

        void setFlipY(bool flipY);
        bool isFlipY() const;

        void setTextureCutFactor(float textureCutFactor);
        float getTextureCutFactor() const;

        void setTextureRect(float x, float y, float width, float height);
        void setTextureRect(Rect textureRect);
        Rect getTextureRect() const;

        void setPivotPreset(PivotPreset pivotPreset);
        PivotPreset getPivotPreset() const;

        void addFrame(int id, std::string name, Rect rect);
        void addFrame(std::string name, float x, float y, float width, float height);
        void addFrame(float x, float y, float width, float height);
        void addFrame(Rect rect);
        void removeFrame(int id);
        void removeFrame(std::string name);

        void setFrame(int id);
        void setFrame(std::string name);

        void startAnimation(std::vector<int> frames, std::vector<int> framesTime, bool loop);
        void startAnimation(int startFrame, int endFrame, int interval, bool loop);
        void startAnimation(std::string name, int interval, bool loop);
        void pauseAnimation();
        void resumeAnimation();
        void stopAnimation();

    };
}

#endif //SPRITE_H