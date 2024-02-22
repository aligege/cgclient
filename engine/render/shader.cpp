#include"shader.h"
#include "global.h"
#include "glad/glad.h"

namespace cg
{
    bool shader::init(const std::string& file)
    {
        if(file.empty())
        {
            global::log.error("load failed:file is empty!");
            return false;
        }
        auto index = file.find_last_of('.');
        if(index!=std::string::npos)
        {
            _file=file.substr(0,index);
        }
        else
        {
            _file=file;
        }
        _pvertsrc = global::loadertool.load<std::string>(_file+".vert", ELoaderType_Shader);
        if(_pvertsrc==nullptr)
        {
            global::log.error("load failed:"+_file);
            return false;
        }
        _pfragsrc = global::loadertool.load<std::string>(_file+".frag", ELoaderType_Shader);
        if(_pfragsrc==nullptr)
        {
            global::log.error("load failed:"+_file);
            delete _pvertsrc;
            return false;
        }
        //创建顶点着色器
        _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        auto vertex_shader_source = _pvertsrc->c_str();
        glShaderSource(_vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(_vertex_shader);
        int  success;
        char infoLog[512];
        glGetShaderiv(_vertex_shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(_vertex_shader, 512, NULL, infoLog);
            std::string info = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
            info += infoLog;
            global::log.error(info);
            return false;
        }
        //创建片段着色器
        _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        auto fragment_shader_source = _pfragsrc->c_str();
        glShaderSource(_fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(_fragment_shader);
        glGetShaderiv(_fragment_shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(_fragment_shader, 512, NULL, infoLog);
            std::string info = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
            info += infoLog;
            global::log.error(info);
            return false;
        }
        //创建着色器程序
        _shader_program = glCreateProgram();
        glAttachShader(_shader_program, _vertex_shader);
        glAttachShader(_shader_program, _fragment_shader);
        glLinkProgram(_shader_program);
        glGetProgramiv(_shader_program, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(_shader_program, 512, NULL, infoLog);
            std::string info = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
            info += infoLog;
            global::log.error(info);
            return false;
        }
        return true;
    }

    void shader::use()
    {
        glUseProgram(_shader_program);
    }

    void shader::end()
    {
        //删除着色器
        glDeleteShader(_vertex_shader);
        glDeleteShader(_fragment_shader);
    }
}