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
            STBText stbtext;
            std::vector<Vector2> charPositions;
            std::string text;
            int width;
            int height; 
            int fixedWidth; 
            int fixedHeight;
            int multiline;
            bool flipY=false;
        public:
            UIText();
        public:
            void init();
    };
}

#endif //UITEXT_H