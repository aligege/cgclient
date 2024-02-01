#ifndef _CG_HTTP_TOOL_H_
#define _CG_HTTP_TOOL_H_

#include "json/jsonObject.h"
#include <unordered_map>

namespace cg
{
    class httpheader
    {
    public:
        char *memory;
        size_t size;
    protected:
        jsonobject jsonheader;
    public:
        httpheader() {
            memory = (char*)malloc(1);
            memory[0]=0;
            size = 0;
        }
        ~httpheader() {
            size=0;
            free(memory);
        }
    public:
        bool parse();
        std::string get(const char* key) {
            auto jo = jsonheader.getObject(key);
            if(jo!=nullptr)
            {
                return jo->getString();
            }
            return "";
        }
    };
    class httpresponse 
    {
    public:
        httpheader* pheader;
        char *memory;
        size_t size;
    public:
        httpresponse() {
            pheader=new httpheader();
            memory = (char*)malloc(1);
            memory[0]=0;
            size = 0;
        }
        ~httpresponse() {
            size=0;
            free(memory);
            delete pheader;
        }
    };
    class httptool
    {
        public:
            httptool();
            virtual ~httptool();
        public:
            void get(const char* url,jsonobject* pjoheader, void (*callback)(httpresponse*));
            httpresponse* post(const char* url, jsonobject* pjodata,jsonobject* pjoheader);
        protected:
            static size_t _onData(void *contents, size_t size, size_t nmemb, void *userp);
            static size_t _onHeader(void *contents, size_t size, size_t nmemb, void *userp);
    };
}

#endif // _CG_HTTP_TOOL_H_