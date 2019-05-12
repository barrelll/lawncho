#include <include/texture_cache.h>

namespace iom
{
    texture_cache::texture_cache()
        :
			_directory_loader(),
            _texturePath("../textures/") // Default param, should be changable later
    {
        cacheInit(_texturePath);
    };

    texture_cache::~texture_cache()
    {
        if (!_textureMap.empty())
            _textureMap.clear();
    };

    void texture_cache::AddTexture(const std::string& imageFilePath)
    {
        auto map_iterator = _textureMap.find(imageFilePath);
        if (map_iterator == _textureMap.end())
        {
            texture* newTexture = new BMP();
            
            _textureMap.insert(std::make_pair(imageFilePath, newTexture));
        }
        else 
        {
            std::cout << "Texture already cached.\n";
        }
    }

    void texture_cache::RemoveTexture(const std::string& imageFilePath)
    {
        auto map_iterator = _textureMap.find(imageFilePath);
        if(map_iterator == _textureMap.end())
        {
            std::cout << "Texture does not exist, exiting.\n";
        }
        else 
        {
            _textureMap.erase(map_iterator);
        }
    }

    void texture_cache::LoadTextures()
    {
        typedef std::map<std::string, texture*>::iterator it_type;
        for (it_type it = _textureMap.begin(); it != _textureMap.end(); it++)
        {
            _textureMap.at(it->first)->LoadTexture(it->first);
        }
    }

    texture* texture_cache::GetCachedTexture(const std::string& imageFilePath)
    {
        if(_textureMap[imageFilePath] != nullptr)
            return _textureMap[imageFilePath];
        else
            return nullptr;
    }

    void texture_cache::cacheInit(const std::string& dir)
    {
/*        DIR *dp;
        dirent *dirp;
        if((dp = opendir(dir.c_str())) == nullptr) {
            std::cout << "Error ( " << errno << " ) opening " << dir << std::endl;
        }

        while ((dirp = readdir(dp)) != nullptr) {
            std::string dirFile = dirp->d_name;
            if (dirFile == "." || dirFile == "..")
                dirFile = "";
            else 
                AddTexture(_texturePath + std::string(dirFile));
        }

        closedir(dp);
*/
		_directory_loader.RecursiveFileLoader(dir);
    }

    texture* texture_cache::operator[] (const std::string& rhs)
    {
        AddTexture(rhs);
        if(_textureMap[rhs] != nullptr)
            return _textureMap[rhs];
        else
            return nullptr;
    }
}
