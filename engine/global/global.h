#include "net/http.h"

namespace cg
{
    class global
    {
        public:
            static http* phttp;
    };
    http* global::phttp = new http();
}