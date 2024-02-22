#pragma once
#include <string>

namespace cg
{
    class iloader
    {
        public:
            /**
             * @brief Construct a new iloader object
            */
            iloader() = default;

            /**
             * @brief Destroy the iloader object
            */
            virtual ~iloader() = default;

            /**
             * @brief Load the file
             * 
             * @param filename The file to load
             * @return true If the file was loaded
             * @return false If the file was not loaded
            */
            virtual void* load(std::string filename) = 0;
    };
}