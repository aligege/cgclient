#ifndef StringUtils_h
#define StringUtils_h

#include <string>

namespace cg{
    class StringUtils{
    public:
        static std::string toUTF8(wchar_t codepoint);
    };
}

#endif /* StringUtils_h */
