#include "http.h"
#include "Log.h"
#include "HTTPRequest.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace http;
namespace cg
{
    http::http()
    {

    }
    http::~http()
    {

    }

    void http::get(const char *url, void (*callback)(const char *))
    {
        try
        {
            Request request(url);
            Response response = request.send("GET");
            if (response.status.code == Status::Ok)
            {
                auto body = (char*)response.body.data();
                Log::debug("http get success:s%",body);
                callback(body);
            }
            else
            {
                Log::debug("http get error");
                callback("http get error");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void http::post(const char *url, void (*callback)(const char *))
    {
        try
        {
            Request request(url);
            Response response = request.send("GET");
            if (response.status.code == Status::Ok)
            {
                auto body = (char*)response.body.data();
                Log::debug("http get success:s%",body);
                callback(body);
            }
            else
            {
                Log::debug("http get error");
                callback("http get error");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}