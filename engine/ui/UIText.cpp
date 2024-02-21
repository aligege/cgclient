#include "UIText.h"
#include "pool/FontPool.h"
#include "Log.h"

namespace cg
{
    UIText::UIText()
    {
    }
    UIText::UIText(std::string text)
    {
        this->text = text;
    }
    void UIText::init()
    {
        this->loadFontAtlas();
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

        this->stbtext->createText(this->text, &this->buffer, indices_array, this->charPositions, this->width, this->height, this->fixedWidth, this->fixedHeight, this->multiline, this->flipY);

        this->indices.setValues(
                0, this->indices.getAttribute(AttributeType::INDEX),
                indices_array.size(), (char*)&indices_array[0], sizeof(uint16_t));
    }
    bool UIText::loadFontAtlas()
    {
        std::string fontId = this->font;
        if (this->font.empty())
            fontId = "font";
        fontId = fontId + std::string("|") + std::to_string(this->fontSize);

        this->stbtext = FontPool::get(fontId);
        if (!this->stbtext){
            this->stbtext = FontPool::get(fontId, this->font, this->fontSize);
            if (!this->stbtext) {
                Log::error("Cannot load font atlas from: %s", this->font.c_str());
                return false;
            }
        }

        this->texture.setData(*this->stbtext->getTextureData(), fontId);
        this->texture.setReleaseDataAfterLoad(true);

        this->needUpdateTexture = true;

        return true;
    }
} // namespace cg