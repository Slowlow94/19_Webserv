#ifndef DIRECTORYHANDLER_HPP
# define DIRECTORYHANDLER_HPP

# include <string>
# include <vector>
# include <dirent.h>
# include <sys/stat.h>
# include <algorithm>
# include <ctime>
# include <sstream>
# include <iostream>
# include "configuration_file_parsing/utils/PathValidator.hpp"
# include "Logger.hpp"

struct FileInfo
{
    std::string _name;
    std::string _size;
    std::string _modified;
    bool        _isDirectory;
    
    FileInfo(const std::string& n, const std::string& s, const std::string& m, bool d);
};

class DirectoryHandler
{
    private:
        PathValidator            _validator;

        std::string				humanReadableSize(size_t bytes);
        std::string				formatTime(time_t time);
        static bool				compareFileInfo(const FileInfo& a, const FileInfo& b);
        std::string				generateBreadcrumbs(const std::string& path);
        
    public:
        DirectoryHandler();
        ~DirectoryHandler();
        std::vector<FileInfo>	getDirectoryListing(const std::string& dir_path);
        std::string				generateDirectoryPage(const std::string& path, const std::vector<FileInfo>& files, const std::string &file_path);
        bool					createDirectory(const std::string& path);
        bool					isDirectory(const std::string& path);
};

#endif
