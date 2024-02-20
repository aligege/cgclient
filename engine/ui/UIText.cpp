#include "UIText.h"

namespace cg
{
    UIText::UIText()
    {
    }
    void UIText::init()
    {
        this->primitiveType = PrimitiveType::TRIANGLES;

        this->buffer.clear();
        this->buffer.addAttribute(AttributeType::POSITION, 3);
        this->buffer.addAttribute(AttributeType::TEXCOORD1, 2);
        this->buffer.setUsage(BufferUsage::DYNAMIC);

        this->indices.clear();
        this->indices.setUsage(BufferUsage::DYNAMIC);

        std::vector<uint16_t> indices_array;

        this->minBufferCount = this->maxTextSize * 4;
        this->minIndicesCount = this->maxTextSize * 6;

        this->stbtext.createText(this->text, &this->buffer, indices_array, this->charPositions, this->width, this->height, this->fixedWidth, this->fixedHeight, this->multiline, this->flipY);

        this->indices.setValues(
                0, this->indices.getAttribute(AttributeType::INDEX),
                indices_array.size(), (char*)&indices_array[0], sizeof(uint16_t));
    }
} // namespace cg