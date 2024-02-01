#include "httptool.h"
#include "Log.h"

#include <iostream>
#include <fstream>
#include <string>
#include "curl/curl.h"
#include "json/jsonobject.h"

namespace cg
{
    bool httpheader::parse()
    {
        if(size==0)
        {
            return false;
        }
        std::string strheader(memory);
        std::string strkey;
        std::string strvalue;
        int index=0;
        int index2=0;
        int index3=0;
        while(index<size)
        {
            index2=strheader.find(": ",index);
            auto dt = 1;
            if(index2==-1)
            {
                dt=0;
                index2=strheader.find(":",index);
                if(index2==-1)
                {
                    break;
                }
            }
            strkey=strheader.substr(index,index2-index);
            auto i = strkey.find_last_of("\r\n");
            if(i!=-1)
            {
                strkey=strkey.substr(i+1);
            }
            index3=strheader.find("\r\n",index2);
            if(index3==-1)
            {
                break;
            }
            strvalue=strheader.substr(index2+1+dt,index3-index2-1-dt);
            auto jo = new jsonobject(strkey.c_str());
            jo->setString(strvalue.c_str());
            jsonheader.addToObject(jo);
            index=index3+2;
        }
        return true;
    }
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
    size_t httptool::_onData(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t realsize = size * nmemb;
        auto pres = (httpresponse *)userp;
        
        // 注意这里根据每次被调用获得的数据重新动态分配缓存区的大小
        char *ptr = (char *)realloc(pres->memory, pres->size + realsize + 1); 
        if(ptr == nullptr) {
            /* out of memory! */ 
            printf("not enough memory (realloc returned NULL)\n");
            return 0;
        }
        
        pres->memory = ptr;
        memcpy(&(pres->memory[pres->size]), contents, realsize);
        pres->size += realsize;
        pres->memory[pres->size] = 0;
        
        return realsize;
    }
    size_t httptool::_onHeader(void *contents, size_t size, size_t nmemb, void *userp)
    {
        size_t realsize = size * nmemb;
        auto pheader = ((httpresponse *)userp)->pheader;
        
        // 注意这里根据每次被调用获得的数据重新动态分配缓存区的大小
        char *ptr = (char *)realloc(pheader->memory, pheader->size + realsize + 1); 
        if(ptr == NULL) {
            /* out of memory! */ 
            printf("not enough memory (realloc returned NULL)\n");
            return 0;
        }
        
        pheader->memory = ptr;
        memcpy(&(pheader->memory[pheader->size]), contents, realsize);
        pheader->size += realsize;
        pheader->memory[pheader->size] = 0;
        
        return realsize;
    }
    void httptool::get(const char *url,jsonobject* pjoheader, void (*callback)(httpresponse*))
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
            plist = curl_slist_append(plist, "Content-Type:application/x-www-form-urlencoded");
            if(pjoheader!=nullptr)
            {
                auto strheader=pjoheader->toString();
                plist = curl_slist_append(plist, strheader.c_str());
            }
            curl_easy_setopt(pcurl, CURLOPT_HTTPHEADER, plist);

            curl_easy_setopt(pcurl, CURLOPT_URL, url);

            curl_easy_setopt(pcurl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(pcurl, CURLOPT_NOSIGNAL, 1);

            auto* pres=new httpresponse();
            curl_easy_setopt(pcurl, CURLOPT_HEADER, 0);
            curl_easy_setopt(pcurl, CURLOPT_HEADERFUNCTION, &httptool::_onHeader);
            curl_easy_setopt(pcurl, CURLOPT_HEADERDATA, pres);
            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, &httptool::_onData);
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, pres);

            curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1);
            curl_easy_setopt(pcurl, CURLOPT_HTTPGET, 1);

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
            pres->pheader->parse();
            callback(pres);
            curl_slist_free_all(plist);
            curl_easy_cleanup(pcurl);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    httpresponse* httptool::post(const char *url, jsonobject* pjodata,jsonobject* pjoheader)
    {
        try
        {
            CURL *pcurl;
            pcurl = curl_easy_init();
            if (NULL == pcurl)
            {
                Log::debug("curl_easy_init error");
                return nullptr;
            }

            curl_slist *plist = nullptr;
            plist = curl_slist_append(plist, "Content-Type:application/x-www-form-urlencoded");
            std::string strheader="";
            if(pjoheader!=nullptr)
            {
                strheader=pjoheader->toString();
                plist = curl_slist_append(plist, strheader.c_str());
            }
            curl_easy_setopt(pcurl, CURLOPT_HTTPHEADER, plist);

            curl_easy_setopt(pcurl, CURLOPT_URL, url);

            auto* pres=new httpresponse();
            curl_easy_setopt(pcurl, CURLOPT_HEADER, 0);
            curl_easy_setopt(pcurl, CURLOPT_HEADERFUNCTION, &httptool::_onHeader);
            curl_easy_setopt(pcurl, CURLOPT_HEADERDATA, pres);
            curl_easy_setopt(pcurl, CURLOPT_WRITEFUNCTION, &httptool::_onData);
            curl_easy_setopt(pcurl, CURLOPT_WRITEDATA, pres);

            curl_easy_setopt(pcurl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(pcurl, CURLOPT_NOSIGNAL, 1);

            //CURLOPT_VERBOSE的值为1时，会显示详细的调试信息
            curl_easy_setopt(pcurl, CURLOPT_VERBOSE, 1);
            //设置请求为post请求
            curl_easy_setopt(pcurl, CURLOPT_POST, 1);
            std::string postJson="";
            if(pjodata!=nullptr)
            {
                postJson=pjodata->toString();
                //设置post请求的参数
                curl_easy_setopt(pcurl, CURLOPT_POSTFIELDS, postJson.c_str());
                curl_easy_setopt(pcurl, CURLOPT_POSTFIELDSIZE, postJson.length());
            }
            Log::debug("post url-%s\nheaders-%s\npostJson-%s\n",url,strheader.c_str(),postJson.c_str());
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
            pres->pheader->parse();
            Log::debug("post url end-%s\ndata-%s\n",url,pres->memory);

            curl_slist_free_all(plist);
            curl_easy_cleanup(pcurl);
            return pres;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}