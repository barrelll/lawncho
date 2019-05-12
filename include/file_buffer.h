#pragma once
#define BOOST_FILESYSTEM_NO_DEPRECATED // We don't want to use any deprecated funcs
#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>

namespace iom
{
    class file_buffer
    {
    public:
        file_buffer();
        file_buffer(const std::string& filePath);
        file_buffer(const file_buffer& rhs);
        ~file_buffer();

        void                        LoadFileBuffer();
        void                        LoadFileBuffer(const std::string& filePath);
        int                         StringToInt(const std::string& rhs);
        float                       StringToFloat(const std::string& rhs);
        /***    This function needs a bit more work, to be of some pretty big help  ***/
        std::string*                RecursiveFileLoader(const std::string& dirPath);
        /***    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  ***/
        std::vector<std::string>    GetFileBuffer()
        { return _fileData; }

        /***    operators! :D   ***/
        file_buffer&        operator= (const file_buffer& rhs);
        file_buffer&        operator+= (const file_buffer& rhs);
        file_buffer&        operator+ (const file_buffer& rhs);

    private:

        std::string         _file;
        std::vector<std::string>  _fileData;
    };
}
