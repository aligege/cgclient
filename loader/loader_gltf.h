#pragma once

#include "iloader.h"
#include<string>
#include "tiny_gltf.h"

class loader_gltf: public iloader
{
    public:
    /**
     * @brief Construct a new loader_gltf object
    */
    loader_gltf() = default;

    /**
     * @brief Destroy the loader_gltf object
    */
    virtual ~loader_gltf() = default;

    /**
     * @brief Load the file
     * 
     * @param filename The file to load
     * @return true If the file was loaded
     * @return false If the file was not loaded
    */
    virtual tinygltf::Model* load(const std::string&  filename);
};