//
// (c) 2021 Eduardo Doria.
//

#ifndef UITEXT_H
#define UITEXT_H

#include "Component.h"
#include "render/Render.h"
#include "buffer/IndexBuffer.h"
#include "buffer/InterleavedBuffer.h"
#include "STBText.h"

namespace cg{
    class UIText: public Component
    {
        protected:
            PrimitiveType primitiveType = PrimitiveType::TRIANGLES;
            InterleavedBuffer buffer;
            IndexBuffer indices;
            unsigned int minBufferCount = 0;
            unsigned int minIndicesCount = 0;
            unsigned int maxTextSize = 100;
            Texture texture;
            std::shared_ptr<STBText> stbtext=nullptr;
            std::vector<Vector2> charPositions;
            std::string text = "";
            std::string font = "";
            unsigned int fontSize = 30;
            int width=200;
            int height=100; 
            bool fixedWidth=false; 
            bool fixedHeight=false;
            bool multiline=false;
            bool flipY=false;
            bool needUpdateTexture = false;
        public:
            UIText();
            UIText(std::string text);
        public:
            void init();
            bool loadFontAtlas();
    };
}

#endif //UITEXT_H