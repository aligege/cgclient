#include"http.h"
#include <iostream>
#include <fstream>
#include <string>

#include <sokol/sokol_fetch.h>
#include <sokol/sokol_log.h>

#define MAX_FILE_SIZE (1024 * 1024)


cg::http::http()
{
    sfetch_desc_t desc = {0};
    desc.logger.func=slog_func;
    sfetch_setup(&desc);
}
cg::http::~http()
{
    sfetch_shutdown();
}

void cg::http::get(const char* url, void (*callback)(const char*))
{
    //通过sokol_fetch实现http请求
    sfetch_request_t fetch = {0};
    fetch.path = url;
    fetch.callback = [](const sfetch_response_t* response) {
        if (response->fetched) {
                // data has been loaded, and is available via the
                // sfetch_range_t struct item 'data':
                const void* ptr = response->data.ptr;
                size_t num_bytes = response->data.size;
            }
            if (response->finished) {
                // the 'finished'-flag is the catch-all flag for when the request
                // is finished, no matter if loading was successful or failed,
                // so any cleanup-work should happen here...
                if (response->failed) {
                    // 'failed' is true in (addition to 'finished') if something
                    // went wrong (file doesn't exist, or less bytes could be
                    // read from the file than expected)
                }
            }
    };

    static uint8_t buf[MAX_FILE_SIZE];

    sfetch_send(&fetch);
    sfetch_dowork();
}

void cg::http::post(const char* url, void (*callback)(const char*))
{
    
}