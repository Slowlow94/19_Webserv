#include "DirectoryHandler.hpp"

FileInfo::FileInfo(const std::string& n, const std::string& s, const std::string& m, bool d)
    : _name(n), _size(s), _modified(m), _isDirectory(d) {}

DirectoryHandler::DirectoryHandler() {}
DirectoryHandler::~DirectoryHandler() {}

std::string DirectoryHandler::humanReadableSize(size_t bytes)
{
    const char* units[] = {"B", "KB", "MB", "GB"};
    size_t unit = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024 && unit < 3)
    {
        size /= 1024;
        unit++;
    }
    std::ostringstream oss;
    oss.precision(2);
    oss.setf(std::ios::fixed);
    oss << size << " " << units[unit];
    return oss.str();
}

std::string DirectoryHandler::formatTime(time_t time)
{
    char buffer[128];
    struct tm* timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

bool DirectoryHandler::compareFileInfo(const FileInfo& a, const FileInfo& b)
{
    if (a._isDirectory != b._isDirectory)
    {
        return a._isDirectory > b._isDirectory;
    }
    return a._name < b._name;
}

std::string DirectoryHandler::generateBreadcrumbs(const std::string& path)
{
    std::string result = "<div class='breadcrumbs'><a href='/'>Home</a>";
    std::string current;
    std::istringstream iss(path);
    std::string token;
    
    while (std::getline(iss, token, '/'))
    {
        if (!token.empty())
        {
            current += "/" + token;
            result += " / <a href='" + current + "'>" + token + "</a>";
        }
    }
    return result + "</div>";
}

std::vector<FileInfo> DirectoryHandler::getDirectoryListing(const std::string& dir_path)
{
    std::vector<FileInfo> files;
    DIR* dir = opendir(dir_path.c_str());
    
    if (dir != NULL)
    {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL)
        {
            std::string name = entry->d_name;
            if (name != "." && name != "..")
            {
                std::string full_path = dir_path + "/" + name;
                struct stat st;
                
                if (stat(full_path.c_str(), &st) == 0)
                {
                    bool is_dir = S_ISDIR(st.st_mode);
                    std::string size;
                    if (is_dir)
                    {
                        size = "-";
                    }
                    else
                    {
                        size = humanReadableSize(st.st_size);
                    }
                    std::string modified = formatTime(st.st_mtime);
                    files.push_back(FileInfo(name, size, modified, is_dir));
                }
            }
        }
        closedir(dir);
    }
    std::sort(files.begin(), files.end(), compareFileInfo);
    return files;
}

std::string DirectoryHandler::generateDirectoryPage(const std::string& path, const std::vector<FileInfo>& files, const std::string &file_path) {
    std::ostringstream html;
    
    html << "<!DOCTYPE html>\n"
         << "<html>\n<head>\n"
         << "<title>Directory Listing - " << path << "</title>\n"
         << "<style>\n"
         << "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n"
         << ".container { max-width: 1200px; margin: 0 auto; background-color: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n"
         << ".breadcrumbs { margin-bottom: 20px; padding: 10px; background: #f8f9fa; border-radius: 4px; }\n"
         << "table { width: 100%; border-collapse: collapse; margin-top: 20px; }\n"
         << "th, td { padding: 12px 8px; text-align: left; border-bottom: 1px solid #ddd; }\n"
         << "th { background-color: #f8f9fa; font-weight: bold; }\n"
         << "tr:hover { background-color: #f8f9fa; }\n"
         << ".folder { color: #2c5aa0; }\n"
         << ".file { color: #444; }\n"
         << ".upload-form { margin: 20px 0; padding: 20px; background: #f8f9fa; border-radius: 4px; border: 1px solid #ddd; }\n"
         << ".upload-form input[type='file'] { display: block; margin: 10px 0; }\n"
         << ".upload-form input[type='submit'] { background: #2c5aa0; color: white; border: none; padding: 8px 16px; border-radius: 4px; cursor: pointer; }\n"
         << ".upload-form input[type='submit']:hover { background: #1e4b8f; }\n"
         << "a { color: #2c5aa0; text-decoration: none; }\n"
         << "a:hover { text-decoration: underline; }\n"
         << "button { padding: 6px 12px; background-color: #dc3545; color: white; border: none; border-radius: 4px; cursor: pointer; }\n"
         << "button:hover { background-color: #c82333; }\n"
         << "</style>\n"
         << "<script>\n"
         << "function deleteFile(event, url) {\n"
         << "    event.preventDefault();\n"
         << "    if (confirm('Are you sure you want to delete this file?')) {\n"
         << "        fetch(url, { method: 'DELETE' })\n"
         << "            .then(response => {\n"
         << "                if (response.ok) {\n"
         << "                    alert('File deleted successfully.');\n"
         << "                    window.location.reload();\n"
         << "                } else {\n"
         << "                    alert('Failed to delete the file.');\n"
         << "                }\n"
         << "            })\n"
         << "            .catch(error => {\n"
         << "                console.error('Error:', error);\n"
         << "                alert('An error occurred while trying to delete the file.');\n"
         << "            });\n"
         << "    }\n"
         << "    return false;\n"
         << "}\n"
         << "</script>\n"
         << "</head>\n<body>\n"
         << "<div class='container'>\n";
    html << generateBreadcrumbs(path);
    html << "<div class='upload-form'>\n"
         << "<h3>Upload File</h3>\n"
         << "<form action='" << path << "' method='POST' enctype='multipart/form-data'>\n"
         << "<input type='file' name='file' required>\n"
         << "<input type='submit' value='Upload'>\n"
         << "</form>\n"
         << "</div>\n";

    html << "<table>\n"
         << "<tr><th>Name</th><th>Size</th><th>Last Modified</th><th>Action</th></tr>\n";

    for (std::vector<FileInfo>::const_iterator it = files.begin(); it != files.end(); ++it)
    {
        const FileInfo& file = *it;
        html << "<tr>\n<td>";
        if (file._isDirectory)
        {
            html << "<span class='folder'>[] </span>";
        }
        else
        {
            html << "<span class='file'>|| </span>";
        }
        std::string link_path;
        std::string actual_path;
        if (path == "/")
        {
            link_path = path + file._name;
            actual_path = file_path + file._name;
        }
        else
        {
            link_path = path + '/' + file._name;
            actual_path = file_path + '/' + file._name;
        }
        if (file._isDirectory)
        {
            html << "<a href='" << link_path << "/'>" << file._name << "</a></td>\n";
        }
        else
        {
            html << "<a href='" << link_path << "' download>" << file._name << "</a></td>\n";
        }
        
        html << "<td>" << file._size << "</td>\n"
             << "<td>" << file._modified << "</td>\n"
             << "<td>\n";
        if (!file._isDirectory)
        {
            _validator.setPath(actual_path);
            bool isValid = _validator.exists() && _validator.isFile() && _validator.isWritable();
            html << "<script>";
            html << "   function deleteElement(filePath) {";
            html << "       fetch(filePath, {";
            html << "           method: 'DELETE',";
            html << "           headers: {";
            html << "               'Content-Type': 'application/json'";
            html << "           },";
            html << "           body: JSON.stringify({ filePath: filePath })";
            html << "       })";
            html << "       .then(response => {";
            html << "           if (response.ok) {";
            html << "               alert('File deleted successfully.');";
            html << "               window.location.reload();";
            html << "           } else {";
            html << "               alert('Failed to delete the file.');";
            html << "           }";
            html << "       })";
            html << "       .catch(error => {";
            html << "           console.error('Error:', error);";
            html << "           alert('An error occurred while deleting the file.');";
            html << "       });";
            html << "   }";
            html << "   function noPermission() {";
            html << "       alert('No permission >:).');";
            html << "   }";
            html << "</script>";
            if (!isValid)
                html << "<button onclick=\"noPermission()\">Delete File</button>";
            else
                html << "<button onclick=\"deleteElement('" << link_path << "')\">Delete File</button>";
        }
        html << "</td>\n</tr>\n";
    }
    html << "</table>\n</div>\n</body>\n</html>";
    return html.str();
}

bool DirectoryHandler::createDirectory(const std::string& path)
{
    return mkdir(path.c_str(), 0755) == 0;
}

bool DirectoryHandler::isDirectory(const std::string& path)
{
    _validator.setPath(path);
    return _validator.isDirectory();
}