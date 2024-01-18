//
// (c) 2023 Eduardo Doria.
//

#ifndef log_h
#define log_h

namespace cg {

    class Log {
    public:
        static void print(const char* text, ...);

        static void verbose(const char* text, ...);
        static void debug(const char* text, ...);
        static void warn(const char* text, ...);
        static void error(const char* text, ...);
    };

}

#endif
