//
// (c) 2021 Eduardo Doria.
//

#ifndef SBSREADER_H
#define SBSREADER_H

#include <string>
#include "ShaderData.h"
#include "io/FileData.h"

namespace cg {

    class SBSReader {
    private:
        ShaderData shaderData;

        bool read(FileData& file);

    public:
        SBSReader();
        virtual ~SBSReader();

        bool read(std::string filepath);
        bool read(std::vector<unsigned char> datashader);

        ShaderData& getShaderData();
    };

}


#endif //SBSREADER_H
