#include"loadertool.h"
#include"loader_text.h"
#include"loader_gltf.h"
// #include"loader_image.h"
// #include"loader_shader.h"
// #include"loader_font.h"
// #include"loader_audio.h"

namespace cg
{
    iloader* loadertool::get_Loader_by_file(const std::string& filename)
    {
        auto index = filename.find_last_of('.');
        if(index==std::string::npos)
        {
            return nullptr;
        }
        auto ext = filename.substr(index+1);
        auto ploader = get_Loader_by_ext(ext);
        return ploader;
    }
    iloader* loadertool::get_Loader_by_ext(const std::string& ext)
    {
        auto type = ELoaderType_Unknown;
        if(ext=="gltf"||ext=="glb")
        {
            type = ELoaderType_GLTF;
        }
        else if(ext=="txt"||ext=="json")
        {
            type = ELoaderType_Text;
        }
        else if(ext=="png"||ext=="jpg"||ext=="jpeg"||ext=="bmp"||ext=="tga")
        {
            type = ELoaderType_Image;
        }
        else if(ext=="vert"||ext=="frag"||ext=="geom"||ext=="comp")
        {
            type = ELoaderType_Shader;
        }
        else if(ext=="ttf"||ext=="otf")
        {
            type = ELoaderType_Font;
        }
        else if(ext=="wav"||ext=="mp3"||ext=="ogg")
        {
            type = ELoaderType_Audio;
        }
        else
        {
            return nullptr;
        }
        auto ploader = get_Loader_by_type(type);
        return ploader;
    }
    iloader* loadertool::get_Loader_by_type(ELoaderType type)
    {
        iloader* ploader = nullptr;
        switch (type)
        {
        case ELoaderType_Text:
        case ELoaderType_Shader:
            ploader = new loader_text();
            break;
        case ELoaderType_GLTF:
            ploader = new loader_gltf();
            break;
        // case ELoaderType_Image:
        //     ploader = new loader_image();
        //     break;
        // case ELoaderType_Font:
        //     ploader = new loader_font();
        //     break;
        // case ELoaderType_Audio:
        //     ploader = new loader_audio();
        //     break;
        default:
            break;
        }
        return ploader;
    }
    template<typename T>
    T* loadertool::load(std::string filename, ELoaderType type)
    {
        iloader* ploader = nullptr;
        if(type!=ELoaderType_Unknown)
        {
            ploader = get_Loader_by_type(type);
        }
        else
        {
            ploader = get_Loader_by_file(filename);
        }
        if(ploader==nullptr)
        {
            return nullptr;
        }
        T* p = ploader->load(filename);
        return p;
    }
}
