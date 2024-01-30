#ifndef __SOCKET_H__
#define __SOCKET_H__

namespace cg
{
    class socket
    {
        public:
            socket();
            virtual ~socket();
        public:
            void connect(const char* url, void (*callback)(const char*));
            void send(const char* url, void (*callback)(const char*));
    };
}

#endif