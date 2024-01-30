namespace cg
{
    class httptool
    {
        public:
            httptool();
            virtual ~httptool();
        public:
            void get(const char* url, void (*callback)(const char*));
            void post(const char* url, void (*callback)(const char*));
    };
}