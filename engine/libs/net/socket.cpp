#include "socket.h"
namespace cg
{
    socket::socket()
    {
    }
    socket::~socket()
    {
    }
    void socket::connect(const char* url, void (*callback)(const char*))
    {
        callback("socket connect");
    }
    void socket::send(const char* url, void (*callback)(const char*))
    {
        callback("socket send");
    }
}