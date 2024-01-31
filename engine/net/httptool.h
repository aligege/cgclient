#ifndef _CG_HTTP_TOOL_H_
#define _CG_HTTP_TOOL_H_

#include "json/jsonObject.h"
namespace cg
{
    class httptool
    {
        public:
            httptool();
            virtual ~httptool();
        public:
            void get(const char* url,jsonobject* pjoheader, void (*callback)(const char*));
            void post(const char* url, jsonobject* pjodata,jsonobject* pjoheader,void (*callback)(const char*));
    };
}

#endif // _CG_HTTP_TOOL_H_