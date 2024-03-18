#ifndef _log_h_
#define _log_h_

#include <string>
#include <iostream>

namespace cg
{
    class log
    {
        public:
            log()=default;
            ~log()=default;
        public:
            void info(const std::string& msg)
            {
                std::cout << "[INFO] " << msg << std::endl;
            }
            void warn(const std::string& msg)
            {
                std::cout << "[WARN] " << msg << std::endl;
            }
            void error(const std::string& msg)
            {
                std::cout << "[ERROR] " << msg << std::endl;
            }
            void fatal(const std::string& msg)
            {
                std::cout << "[FATAL] " << msg << std::endl;
                exit(1);
            }
    };
}

#endif