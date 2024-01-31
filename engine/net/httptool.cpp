#include "httptool.h"
#include "Log.h"
#include "HTTPRequest.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include "curl/curl.h"

using namespace http;
namespace cg
{
    httptool::httptool()
    {
        CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
        if (res != CURLE_OK)
        {
            Log::debug("curl_global_init error");
        }
    }
    httptool::~httptool()
    {
        curl_global_cleanup();
    }

    void httptool::get(const char *url, void (*callback)(const char *))
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

    void httptool::post(const char *url, const char* jsonStr, void (*callback)(const char *))
    {
        try
        {
            CURL *pcurl;
            pcurl = curl_easy_init();
            if (NULL == pcurl)
            {
                Log::debug("curl_easy_init error");
                return;
            }
            curl_slist *plist = nullptr;
            plist = curl_slist_append(plist, "Content-Type:application/x-www-form-urlencoded;charset=UTF-8");
            plist = curl_slist_append(plist, "Accept:application/json, text/javascript, */*; q=0.01");
            plist = curl_slist_append(plist, "Accept-Language:zh-CN,zh;q=0.8");
            plist = curl_slist_append(plist, "Uid:1008611");
            curl_easy_setopt(pcurl, CURLOPT_HTTPHEADER, plist);

            curl_easy_setopt(pcurl, CURLOPT_URL, url);

            curl_easy_setopt(pcurl, CURLOPT_HEADER, 0);
            curl_easy_setopt(pcurl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(pcurl, CURLOPT_NOSIGNAL, 1);

            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, callback);
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, (void *)callback);

            curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1);

            auto len=strlen(jsonStr);
            if(len>0)
            {
                curl_easy_setopt(pcurl, CURLOPT_POST, 1);
                curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, jsonStr);
                curl_easy_setopt(pcurl, CURLOPT_POSTFIELDSIZE, len);
            }

            CURLcode res = curl_easy_perform(pcurl);

            if (res != CURLE_OK)
            {
                Log::debug("curl_easy_perform error");
            }
            long response_code=0;
            res=curl_easy_getinfo(pcurl, CURLINFO_RESPONSE_CODE, &response_code);
            if((res == CURLE_OK) && (response_code==200||response_code==201))
            {
                Log::debug("curl_easy_getinfo success");
            }
            else
            {
                Log::debug("curl_easy_getinfo error");
            }

            curl_slist_free_all(plist);
            curl_easy_cleanup(pcurl);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}