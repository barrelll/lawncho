#include <include/BMP.h>

namespace iom
{
    BMP::BMP()
    {
	    _textureID 	= 0;
	    width 	= 0;
	    height 	= 0;
    };

    BMP::~BMP()
    {
	    if (_textureID)
		    glDeleteTextures(1, &_textureID);
	    width = 0;
	    height = 0;
    };

    void BMP::LoadTexture(const std::string& imageFilePath)
    {
	iom::file_buffer _file;
        _file.LoadFileBuffer(imageFilePath);

        std::string file = _file.GetFileBuffer().back();

        if (file.empty())
        {
            printf("Bitmap file couldn't load, exiting.\n");
            glfwTerminate();
        }

        if (file.at(0) != 'B' && file.at(1) != 'M')
        {
            printf("Not a correct Bitmap file, exiting.\n");
            glfwTerminate();
        }

        width  = *(int*)&file.at(0x12); 
        height = *(int*)&file.at(0x16); 

        unsigned int imageSize = (width * height) * 3;

		std::vector<GLubyte> data;
		data.resize(imageSize);
        // Read the actual data from the file into the buffer
        for (unsigned int i = 0; i < imageSize; i++)
        {
            data[i] = *(GLubyte*)&file.at(i);
        }

        /***    need to test this for context before use   ***/
        // Create one OpenGL texture
        glGenTextures(1, &_textureID);
       
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, _textureID);
       
        // Give the image to OpenGL
	glGenerateMipmap(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data());
       
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
