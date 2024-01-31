#include "httptool.h"
#include "Log.h"

#include <iostream>
#include <fstream>
#include <string>
#include "curl/curl.h"
#include "json/jsonobject.h"

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
    //get请求和post请求数据响应函数
    size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        //在注释的里面可以打印请求流，cookie的信息
        std::string *str = (std::string*)stream;
        (*str).append((char*)ptr, size*nmemb);
        return size * nmemb;
    }
    void httptool::get(const char *url,jsonobject* pjoheader, void (*callback)(const char *))
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
            if(pjoheader!=nullptr)
            {
                const char* strheader=pjoheader->toString();
                plist = curl_slist_append(plist, strheader);
            }
            curl_easy_setopt(pcurl, CURLOPT_HTTPHEADER, plist);

            curl_easy_setopt(pcurl, CURLOPT_URL, url);

            curl_easy_setopt(pcurl, CURLOPT_HEADER, 0);
            curl_easy_setopt(pcurl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(pcurl, CURLOPT_NOSIGNAL, 1);

            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, callback);
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, (void *)callback);

            curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1);

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
    void httptool::post(const char *url, jsonobject* pjodata,jsonobject* pjoheader, void (*callback)(const char *))
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
            if(pjoheader!=nullptr)
            {
                const char* strheader=pjoheader->toString();
                plist = curl_slist_append(plist, strheader);
            }
            curl_easy_setopt(pcurl, CURLOPT_HTTPHEADER, plist);

            curl_easy_setopt(pcurl, CURLOPT_URL, url);

            ////不接收响应头数据0代表不接收 1代表接收
            curl_easy_setopt(pcurl, CURLOPT_HEADER, 0);
            curl_easy_setopt(pcurl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(pcurl, CURLOPT_NOSIGNAL, 1);

            //设置数据接收和写入函数
            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, callback);
            //todo 这里有问题
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, (void *)callback);

            //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
            curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1);
            //设置请求为post请求
            curl_easy_setopt(pcurl, CURLOPT_POST, 1);

            if(pjodata!=nullptr)
            {
                const char* jsonStr=pjodata->toString();
                //设置post请求的参数
                curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, jsonStr);
                curl_easy_setopt(pcurl, CURLOPT_POSTFIELDSIZE, strlen(jsonStr));
            }

            //设置超时时间
            //curl_easy_setopt(pcurl, CURLOPT_CONNECTTIMEOUT, 6);
            //curl_easy_setopt(pcurl, CURLOPT_TIMEOUT, 6);

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