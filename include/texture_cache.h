#pragma once
#include <include/texture.h>
#include <include/BMP.h>
#include <map>
#include <iostream>

namespace iom
{
    class texture_cache 
    {
    public:
        texture_cache();
        ~texture_cache();

        void        AddTexture(const std::string& imageFilePath);
        void        RemoveTexture(const std::string& imageFilePath);
        void        LoadTextures();
        texture*    GetCachedTexture(const std::string& imageFilePath);

        texture*    operator[] (const std::string& rhs);
    private:
        texture_cache(const texture_cache& rhs);            // No reason to copy to other classes
        texture_cache operator= (const texture_cache& rhs); //

        /***    Will load all textures in a directory on object creation   ***/
        void        cacheInit(const std::string& dir);

		file_buffer _directory_loader;
        std::string _texturePath;
        std::map<std::string, texture*> _textureMap;
    };
}
