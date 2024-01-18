//
// (c) 2023 Eduardo Doria.
//

#ifndef INTERLEAVEDBUFFER_H
#define INTERLEAVEDBUFFER_H

#include <string>
#include <vector>
#include <map>

#include "buffer/Buffer.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

namespace cg{

    class InterleavedBuffer: public Buffer{

    private:
        std::vector<unsigned char> vectorBuffer;
        unsigned int vertexSize;

    public:
        InterleavedBuffer();
        virtual ~InterleavedBuffer();

        InterleavedBuffer(const InterleavedBuffer& rhs);
        InterleavedBuffer& operator=(const InterleavedBuffer& rhs);

        virtual bool resize(size_t pos);
        virtual void clearAll();
        virtual void clear();

        void addAttribute(AttributeType attribute, int elements);

    };

}


#endif //INTERLEAVEDBUFFER_H
