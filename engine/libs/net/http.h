namespace cg
{
    class http
    {
        public:
            http();
            virtual ~http();
        public:
            void get(const char* url, void (*callback)(const char*));
            void post(const char* url, void (*callback)(const char*));
    };
}