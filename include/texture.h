#pragma once
#include <include/file_buffer.h>
#include <include/GL/glew.h>
#include <include/GLFW/glfw3.h>
#include <vector>
#include <string>

namespace iom
{
    class texture
    {
    public:
        virtual void    LoadTexture(const std::string& rhs) = 0;

        void            UseTexture() const
        { glBindTexture(GL_TEXTURE_2D, _textureID); }

        void            UnuseTexture() const
        { glBindTexture(GL_TEXTURE_2D, 0); }

    protected:
        GLuint _textureID;
    };
}
