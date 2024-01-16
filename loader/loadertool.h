#pragma once

#include <unordered_map>
#include <string>
#include"iloader.h"

enum ELoaderType
{
    ELoaderType_Unknown,
    ELoaderType_Text,
    ELoaderType_Binary,
    ELoaderType_Image,
    ELoaderType_Audio,
    ELoaderType_GLTF,
    ELoaderType_Shader,
    ELoaderType_Font,
    ELoaderType_Audio
};
class loadertool
{
    protected:
        /**
         * @brief Construct a new loadertool object
        */
        std::unordered_map<std::string, std::string> m_map;
    protected:
        iloader* get_Loader_by_file(const std::string& filename);
        iloader* get_Loader_by_ext(const std::string& ext);
        iloader* get_Loader_by_type(ELoaderType type);
        loadertool() = default;
    public:
        /**
         * @brief Destroy the loadertool object
        */
        virtual ~loadertool() = default;

        /**
         * @brief Load the file
         * 
         * @param filename The file to load
         * @return true If the file was loaded
         * @return false If the file was not loaded
        */
       template<typename T>
        T* load(const std::string& filename, ELoaderType type = ELoaderType_Unknown);
};