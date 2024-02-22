#pragma once

#include "shader.h"

namespace cg
{
    class render_object
    {
        protected:
            shader*  _pshader;
            unsigned int _vbo;
            unsigned int _vao;
            unsigned int _ebo;
            float* _vertices;
            unsigned int* _indices;
        public:
            render_object() = default;
            virtual ~render_object() = default;
        public:
            bool init(std::string& shader_path,float* vertices, unsigned int* indices);
            void renderVBO();
            void renderVAO();
            void renderVEO();
    };
}