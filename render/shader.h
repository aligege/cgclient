#pragma once

#include<string>
class shader
{
    protected:
        std::string _file;
        std::string* _pvertsrc=nullptr;
        std::string* _pfragsrc=nullptr;
        unsigned int _vertex_shader;
        unsigned int _fragment_shader;
        unsigned int _shader_program;
    public:
        shader() = default;
        virtual ~shader()
        {
            if(_pvertsrc!=nullptr)
            {
                delete _pvertsrc;
                _pvertsrc=nullptr;
            }
            if(_pfragsrc!=nullptr)
            {
                delete _pfragsrc;
                _pfragsrc=nullptr;
            }
        }
    public:
        bool init(const std::string& file);
        void use();
        void end();
};