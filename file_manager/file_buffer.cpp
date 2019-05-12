#include <include/file_buffer.h>

namespace iom
{
    file_buffer::file_buffer(const std::string& filePath)
        :
            _file(std::move(filePath)),
            _fileData(0)
    {};

    file_buffer::file_buffer()
        :
            _file(""),
            _fileData(0)
    {};

    file_buffer::file_buffer(const file_buffer& rhs)
        :
            _file(std::move(rhs._file)),
            _fileData(std::move(rhs._fileData))
    {};

    file_buffer::~file_buffer()
    {
        if (!_file.empty())
        {
            _file.erase(_file.begin(), _file.end());
        }
        if (!_fileData.empty())
        {
            _fileData.erase(_fileData.begin(), _fileData.end());
        }
    };

    void file_buffer::LoadFileBuffer()
    {
         std::ifstream file(_file);
         if (!file)
             return;

         std::string buffer;
         while (std::getline(file, buffer))
         {
             if (buffer.empty())
                 continue;

             _fileData.push_back(buffer);
         }
     
         file.close();
    }

    void file_buffer::LoadFileBuffer(const std::string& filePath)
    {
        _file = filePath;
        std::ifstream file(_file);
        if (!file)
            std::cout << "File is empty.\n";

        std::string buffer;
        while (std::getline(file, buffer))
        {
            if (buffer.empty())
                continue;

            _fileData.push_back(buffer);
        }
     
        file.close();
    }

	// Probably should be using a std::vector<std::string> type instead of returning a pointer list
    std::string* file_buffer::RecursiveFileLoader(const std::string& dirPath)
    {
		boost::progress_timer t(std::clog);

		boost::filesystem::path full_path(boost::filesystem::initial_path<boost::filesystem::path>());

		full_path = boost::filesystem::system_complete(boost::filesystem::path(dirPath));

		unsigned long file_count = 0;
		unsigned long dir_count = 0;
		unsigned long other_count = 0;
		unsigned long err_count = 0;

		if (!boost::filesystem::exists(full_path))
		{
			std::cout << "\nNot found: " << full_path.filename() << std::endl;
			return 0;
		}

		if (boost::filesystem::is_directory(full_path))
		{
			std::cout << "\nIn directory: "
				<< full_path.string() << "\n\n";
			boost::filesystem::directory_iterator end_iter;
			for (boost::filesystem::directory_iterator dir_itr(full_path);
				dir_itr != end_iter;
				++dir_itr)
			{
				try
				{
					if (boost::filesystem::is_directory(dir_itr->status()))
					{
						++dir_count;
						std::cout << dir_itr->path().filename() << " [directory]\n";
					}
					else if (boost::filesystem::is_regular_file(dir_itr->status()))
					{
						++file_count;
						std::cout << dir_itr->path().filename() << "\n";
					}
					else
					{
						++other_count;
						std::cout << dir_itr->path().filename() << " [other]\n";
					}

				}
				catch (const std::exception & ex)
				{
					++err_count;
					std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
				}
			}
			std::cout << "\n" << file_count << " files\n"
				<< dir_count << " directories\n"
				<< other_count << " others\n"
				<< err_count << " errors\n";
		}
		else // must be a file
		{
			std::cout << "\nFound: " << full_path.filename() << "\n";
		}
		return 0;
    }

    /***
     * Maybe make the below functions a bit more dynamic, 
     * and deal with different file types
     ***/

    int file_buffer::StringToInt(const std::string& rhs)
    { int si = std::stoi(rhs); return si; }

    float file_buffer::StringToFloat(const std::string& rhs)
    { float sf = std::stof(rhs); return sf; }

    /***************************************/

    file_buffer& file_buffer::operator= (const file_buffer& rhs)
    {
        this->_file     = rhs._file;       
        this->_fileData = rhs._fileData;
        return *this;
    }

    // Appends the end of right hand side to *this
    file_buffer& file_buffer::operator+= (const file_buffer& rhs)
    {
        // Filepath shouldn't be relevant when adding the contents of each file
        // append the vector,
        if (!rhs._fileData.empty())
        {
            this->_fileData.insert(_fileData.begin(), rhs._fileData.begin(), rhs._fileData.end());
            return *this;
        }
        else if (this->_fileData.empty() && rhs._fileData.empty())
        {
            printf("Both file buffers are empty.\nLoad both files before appending.\n");
        }
        else if (rhs._fileData.empty())
        {
            printf("%s file buffer is empty, nothing to append\n", rhs._file.c_str());
        }
        else
        {
            printf("File buffer is empty.\nLoad both files before appending.\n");
        }
        return *this;
    }

    // Essentially the same as +=, just here for a just in case I don't type += deal
    file_buffer& file_buffer::operator+ (const file_buffer& rhs)
    {
        // Filepath shouldn't be relevant when adding the contents of each file
        // append the vector,
        if (!rhs._fileData.empty())
        {
            this->_fileData.insert(_fileData.begin(), rhs._fileData.begin(), rhs._fileData.end());
            return *this;
        }
        else if (this->_fileData.empty() && rhs._fileData.empty())
        {
            printf("Both file buffers are empty.\nLoad both files before appending.\n");
        }
        else if (rhs._fileData.empty())
        {
            printf("%s file buffer is empty, nothing to append\n", rhs._file.c_str());
        }
        else
        {
            printf("File buffer is empty.\nLoad both files before appending.\n");
        }
        return *this;
    }
}
