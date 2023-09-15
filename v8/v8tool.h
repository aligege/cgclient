#include <v8/v8.h>
#include <v8/libplatform/libplatform.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class v8tool
{
    // 这里就是一个创建上下问的方法，我只需要一个上下文，你可以简单理解为定义全局变量
    static v8::Local<v8::Context> CreateContext(v8::Isolate *isolate);
    // 这里就是用于执行命令行的文件的功能
    static int RunMain(v8::Isolate *isolate, v8::Platform *platform, int argc,
                char *argv[]);
    // 用于执行js字符串的功能
    static bool ExecuteString(v8::Isolate *isolate, v8::Local<v8::String> source,
                       v8::Local<v8::Value> name, bool print_result,
                       bool report_exceptions);
    // 内嵌到js代码里面的功能，你可以理解为fs.readFileSync
    static void ReadFile(const v8::FunctionCallbackInfo<v8::Value> &args);
    static v8::MaybeLocal<v8::String> ReadCommandFile(v8::Isolate *isolate, const char *name);
    // 用于异常的打印
    static void ReportException(v8::Isolate *isolate, v8::TryCatch *handler);
};