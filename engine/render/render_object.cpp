#include"render_object.h"
#include"glad/glad.h"
#include"shader.h"

namespace cg
{
    bool render_object::init(std::string& shader_path,float* vertices, unsigned int* indices)
    {
        if(vertices==nullptr
            ||indices==nullptr)
        {
            return false;
        }
        this->_vertices=vertices;
        this->_indices=indices;
        this->_pshader = new shader();
        auto ret = this->_pshader->init(shader_path);
        if(!ret)
        {
            delete this->_pshader;
            return false;
        }

        // 1. 绑定顶点数组对象
        glGenBuffers(1, &_vbo);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->_vertices), this->_vertices, GL_STATIC_DRAW);
        // 3. 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // 4. 解析顶点数据
        glEnableVertexAttribArray(0);
        return true;
    }

    void render_object::renderVBO()
    {
        // 5. 绘制物体
        this->_pshader->use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void render_object::renderVAO()
    {
        if(this->_vertices==nullptr
            ||this->_pshader==nullptr)
        {
            return;
        }
        // 1. 绑定顶点数组对象
        glGenVertexArrays(1, &_vao);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->_vertices), this->_vertices, GL_STATIC_DRAW);
        // 3. 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // 4. 解析顶点数据
        glEnableVertexAttribArray(0);
        // 5. 绘制物体
        this->_pshader->use();
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void render_object::renderVEO()
    {
        if(this->_vertices==nullptr
            ||this->_indices==nullptr
            ||this->_pshader==nullptr)
        {
            return;
        }
        // 1. 绑定顶点数组对象
        glBindVertexArray(_vao);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->_vertices), this->_vertices, GL_STATIC_DRAW);
        // 3. 复制我们的索引数组到一个索引缓冲中
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->_indices), this->_indices, GL_STATIC_DRAW);
        // 3. 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // 4. 解析顶点数据
        glEnableVertexAttribArray(0);
        // 5. 绘制物体
        this->_pshader->use();
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}