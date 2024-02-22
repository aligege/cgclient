#pragma once

#include "iloader.h"

#include<string>

namespace cg
{
    class loader_text: public iloader
    {
        public:
            /**
             * @brief Construct a new loader_text object
            */
            loader_text() = default;

            /**
             * @brief Destroy the loader_text object
            */
            virtual ~loader_text() = default;

            /**
             * @brief Load the file
             * 
             * @param filename The file to load
             * @return true If the file was loaded
             * @return false If the file was not loaded
            */
            virtual void* load(std::string filename);
    };
}