//
// (c) 2023 Eduardo Doria.
//

#include "IndexBuffer.h"
#include "render/ObjectRender.h"

using namespace cg;

IndexBuffer::IndexBuffer(): Buffer(){
    createIndexAttribute();

    type = BufferType::INDEX_BUFFER;

    data = vectorBuffer.data();
}

IndexBuffer::~IndexBuffer(){

}

IndexBuffer::IndexBuffer(const IndexBuffer& rhs): Buffer(rhs){
    vectorBuffer = rhs.vectorBuffer;

    data = vectorBuffer.data();
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& rhs){
    Buffer::operator =(rhs);

    vectorBuffer = rhs.vectorBuffer;

    data = vectorBuffer.data();

    return *this;
}

void IndexBuffer::createIndexAttribute(){
    Buffer::addAttribute(AttributeType::INDEX, AttributeDataType::UNSIGNED_SHORT, 1, 0);
    Buffer::setStride(sizeof(uint16_t));
}

bool IndexBuffer::resize(size_t pos) {
    Buffer::resize(pos);

    if (pos > vectorBuffer.size()) {
        vectorBuffer.resize(pos);

        data = vectorBuffer.data();
        size = vectorBuffer.size();
    }

    return true;
}

void IndexBuffer::clearAll(){
    Buffer::clearAll();

    vectorBuffer.clear();
}

void IndexBuffer::clear(){
    Buffer::clear();
}
