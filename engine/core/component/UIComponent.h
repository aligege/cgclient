//
// (c) 2022 Eduardo Doria.
//

#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "buffer/Buffer.h"
#include "texture/Texture.h"
#include "render/ObjectRender.h"
#include "buffer/InterleavedBuffer.h"
#include "buffer/IndexBuffer.h"

namespace cg{

    struct UIComponent{
        // Render part
        bool loaded = false;
        bool loadCalled = false;

        InterleavedBuffer buffer;
        IndexBuffer indices;

        unsigned int minBufferCount = 0;
        unsigned int minIndicesCount = 0;

        ObjectRender render;
        std::shared_ptr<ShaderRender> shader;
        std::string shaderProperties;
        int slotVSParams = -1;
        int slotFSParams = -1;

        PrimitiveType primitiveType = PrimitiveType::TRIANGLES;
        unsigned int vertexCount = 0;

        Texture texture;
        Vector4 color = Vector4(1.0, 1.0, 1.0, 1.0); //linear color

        FunctionSubscribe<void()> onGetFocus;
        FunctionSubscribe<void()> onLostFocus;
        FunctionSubscribe<void(float, float)> onPointerMove;
        FunctionSubscribe<void(float, float)> onPointerDown;
        FunctionSubscribe<void(float, float)> onPointerUp;

        bool automaticFlipY = true;
        bool flipY = false;

        bool pointerMoved = false;
        bool focused = false;

        bool needReload = false;

        bool needUpdateBuffer = false;
        bool needUpdateTexture = false;
    };

}

#endif //UI_COMPONENT_H