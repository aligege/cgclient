#pragma once

#include <string>
#include <iostream>
class log
{
    public:
        void info(const std::string& msg);
        void warn(const std::string& msg);
        void error(const std::string& msg);
        void fatal(const std::string& msg);
};
void log::info(const std::string& msg)
{
    std::cout << "[INFO] " << msg << std::endl;
}
void log::warn(const std::string& msg)
{
    std::cout << "[WARN] " << msg << std::endl;
}
void log::error(const std::string& msg)
{
    std::cout << "[ERROR] " << msg << std::endl;
}
void log::fatal(const std::string& msg)
{
    std::cout << "[FATAL] " << msg << std::endl;
    exit(1);
}