//参考：https://cnodejs.org/topic/5b49d3a0fb9e84ec69cc1a72
#include <v8tool.h>
// 这里就是一个创建上下问的方法，我只需要一个上下文，你可以简单理解为定义全局变量
v8::Local<v8::Context> v8tool::CreateContext(v8::Isolate *isolate)
{
    // 新建一个js的Object，Template其实就是C++版本的对象的意思
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // 将这个对象绑定一个方法，这个方法在js里面叫readfile，绑定的方法就是上面讲的ReadFile
    // FunctionTemplate和上面Template可以类推
    global->Set(
        v8::String::NewFromUtf8(isolate, "readfile", v8::NewStringType::kNormal)
            .ToLocalChecked(),
        v8::FunctionTemplate::New(isolate, v8tool::ReadFile));
    // 把上下文初始化出来
    return v8::Context::New(isolate, NULL, global);
}
// 这里就是用于执行命令行的文件的功能
int v8tool::RunMain(v8::Isolate *isolate, v8::Platform *platform, int argc,
                    char *argv[])
{
    // 这里取出了命令行的第二个参数，为什么是第二个？
    // ./zy_node test.js 因为第一个是它本身啊
    const char* str = argv[1];
    // 将字符串转成v8的类型
    v8::Local<v8::String> file_name =
        v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal)
            .ToLocalChecked();
    // 定义js文件内容的变量
    v8::Local<v8::String> source;
    // 用ReadCommandFile读取文件到source，并判断是否读取异常
    if (!ReadCommandFile(isolate, str).ToLocal(&source)) {
        fprintf(stderr, "Error reading '%s'\n", str);
        return 0;
    }
    // 开始执行字符串用js引擎，可以直接往下看到ExecuteString方法
    bool success = ExecuteString(isolate, source, file_name, false, true);
    // 这个就是从事件循环（大名顶顶的EVENT_LOOP）中，执行异步函数
    while (v8::platform::PumpMessageLoop(platform, isolate)) continue;
    // 如果失败返回1
    if (!success) return 1;
    return 0;
}
// 用于执行js字符串的功能
bool v8tool::ExecuteString(v8::Isolate *isolate, v8::Local<v8::String> source,
                           v8::Local<v8::Value> name, bool print_result,
                           bool report_exceptions)
{
    // 这就是我之前说的“杯子”，不解释第二次了，往上看
    v8::HandleScope handle_scope(isolate);
    // 外面包个try...catch
    v8::TryCatch try_catch(isolate);
    // 将js文件文件名传入，初始化脚本
    v8::ScriptOrigin origin(name);
    // 直接拿当前隔离层的上下文，作为当前的上下文
    v8::Local<v8::Context> context(isolate->GetCurrentContext());
    // 定义脚本变量
    v8::Local<v8::Script> script;
    // 编译文件代码，成功后并传入script变量中，异常则报错
    if (!v8::Script::Compile(context, source, &origin).ToLocal(&script))
    {
        // report_exceptions 是传入的值，用于报错的开关
        if (report_exceptions)
            ReportException(isolate, &try_catch);
        return false;
    }
    else
    {
        // 定义执行结果
        v8::Local<v8::Value> result;
        // 运行之前编译成功的script，如果运行失败则报错
        // ！！！！注意这行代码就是在运行了
        if (!script->Run(context).ToLocal(&result))
        {
            assert(try_catch.HasCaught());
            // Print errors that happened during execution.
            if (report_exceptions)
                ReportException(isolate, &try_catch);
            return false;
        }
        else
        {
            // 判断确实没有错误，则往下运行
            assert(!try_catch.HasCaught());
            // 是否打印结果，print_result也是一个传入的开关
            if (print_result && !result->IsUndefined())
            {
                // If all went well and the result wasn't undefined then print
                // the returned value.
                v8::String::Utf8Value str(isolate, result);
                const char *cstr = *str;
                printf("%s\n", cstr);
            }
            return true;
        }
    }
}
// 内嵌到js代码里面的功能，你可以理解为fs.readFileSync
void v8tool::ReadFile(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    // 判断参数是不是只有一个
    if (args.Length() != 1)
    {
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Bad parameters",
                                    v8::NewStringType::kNormal)
                .ToLocalChecked());
        return;
    }
    // 将文件名转换成v8的字符串
    v8::String::Utf8Value file(args.GetIsolate(), args[0]);
    if (*file == NULL)
    {
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Error file name",
                                    v8::NewStringType::kNormal)
                .ToLocalChecked());
        return;
    }
    // 定义文件里面的代码数据的变量
    v8::Local<v8::String> source;
    // 调用ReadCommandFile读出文件代码，并检查是否读取成功，ReadCommandFile方法后面会讲到
    if (!ReadCommandFile(args.GetIsolate(), *file).ToLocal(&source))
    {
        args.GetIsolate()->ThrowException(
            v8::String::NewFromUtf8(args.GetIsolate(), "Error loading file",
                                    v8::NewStringType::kNormal)
                .ToLocalChecked());
        return;
    }
    // 将文件的代码数据返回
    args.GetReturnValue().Set(source);
}
v8::MaybeLocal<v8::String> ReadCommandFile(v8::Isolate *isolate, const char *name)
{
    FILE *file = fopen(name, "rb");
    if (file == NULL)
        return v8::MaybeLocal<v8::String>();

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *chars = new char[size + 1];
    chars[size] = '\0';
    for (size_t i = 0; i < size;)
    {
        i += fread(&chars[i], 1, size - i, file);
        if (ferror(file))
        {
            fclose(file);
            return v8::MaybeLocal<v8::String>();
        }
    }
    fclose(file);
    // 这个就是把char转换成v8的字符串数据，实际上这是一个Handle
    v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(
        isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
    delete[] chars;
    return result;
}
// 用于异常的打印
void v8tool::ReportException(v8::Isolate *isolate, v8::TryCatch *handler)
{
}