#pragma once

#include <string>

namespace cg
{
    class core
    {
        public:
            bool startsWith(const std::string& str, const std::string prefix) 
            {
                return (str.rfind(prefix, 0) == 0);
            }

            bool endsWith(const std::string& str, const std::string suffix) 
            {
                if (suffix.length() > str.length()) { return false; }

                return (str.rfind(suffix) == (str.length() - suffix.length()));
            }
    };
}