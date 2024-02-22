#include"loader_gltf.h"
#include "global.h"

namespace cg
{
    void* loader_gltf::load(std::string filepath)
    {
        if(filepath.empty())
        {
            return nullptr;
        }
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;
        tinygltf::Model* pmodel=new tinygltf::Model;
        bool ret = false;
        if(global::core.endsWith(filepath, ".glb"))
        {
            // Assume binary glTF.
            ret = loader.LoadBinaryFromFile(pmodel, &err, &warn, filepath);
        }
        else if(global::core.endsWith(filepath, ".gltf"))
        {
            // Assume ascii glTF.
            ret = loader.LoadASCIIFromFile(pmodel, &err, &warn, filepath);
        }
        else
        {
            // Unknown file type.
            delete pmodel;
            pmodel=nullptr;
            return nullptr;
        }
        return pmodel;
    }
}