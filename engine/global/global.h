#include "../net/httptool.h"

namespace cg
{
    class global
    {
        public:
            static httptool* phttptool;
    };
    httptool* global::phttptool = new httptool();
}