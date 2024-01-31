namespace cg
{
    class httptool
    {
        public:
            httptool();
            virtual ~httptool();
        public:
            void get(const char* url, void (*callback)(const char*));
            void post(const char* url, const char* jsonStr,std::unordered_map<std::string, std::string>& headers,void (*callback)(const char*));
    };
}