#include "HttpRequestHandler.hpp"
#include "RequestResponseState.hpp"

std::map<std::string, std::vector<std::string> > HttpRequestHandler::getLocInfoByUri(HttpRequestHandler request)
{
    std::string requestUri = request.getPath();
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator matchedLocation = _locInfo.end();
    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator it = _locInfo.begin(); it != _locInfo.end(); ++it)
    {
        const std::string& locationUri = it->first;
        if (requestUri.find(locationUri) == 0 && 
            (matchedLocation == _locInfo.end() || locationUri.size() > matchedLocation->first.size()))
        {
            matchedLocation = it;
        }
    }
    if (matchedLocation != _locInfo.end())
    {
        const std::map<std::string, std::vector<std::string> >& locConfig = matchedLocation->second;
        this->setIsValid(true);
        return locConfig;
    }
    this->setIsValid(false);
    return std::map<std::string, std::vector<std::string> >();
}

std::vector<std::string> HttpRequestHandler::getContentPathsFromLoc(const std::string& uri) const
{
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator matchedLocation = _locInfo.end();
    std::string requestUri = uri;
    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator it = _locInfo.begin(); it != _locInfo.end(); ++it)
    {
        const std::string& locationUri = it->first;
        if (requestUri.find(locationUri) == 0 && 
            (matchedLocation == _locInfo.end() || locationUri.size() > matchedLocation->first.size()))
        {
            matchedLocation = it;
        }
    }
    if (matchedLocation == _locInfo.end())
        return std::vector<std::string>();
    const std::map<std::string, std::vector<std::string> >& locConfig = matchedLocation->second;
    std::map<std::string, std::vector<std::string> >::const_iterator contentPathIt = locConfig.find("content_path");
    if (contentPathIt != locConfig.end())
    {
        return contentPathIt->second;
    }
    return std::vector<std::string>();
}

std::string HttpRequestHandler::getRootDirectoryFromLoc(const std::string& uri) const
{
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator matchedLocation = _locInfo.end();
    std::string requestUri = uri;
    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator it = _locInfo.begin(); it != _locInfo.end(); ++it)
    {
        const std::string& locationUri = it->first;
        if (requestUri.find(locationUri) == 0 && 
            (matchedLocation == _locInfo.end() || locationUri.size() > matchedLocation->first.size()))
        {
            matchedLocation = it;
        }
    }
    if (matchedLocation != _locInfo.end())
    {
        const std::map<std::string, std::vector<std::string> >& locConfig = matchedLocation->second;
        std::map<std::string, std::vector<std::string> >::const_iterator rootDirIt = locConfig.find("root_directory");
        if (rootDirIt != locConfig.end() && !rootDirIt->second.empty())
        {
            return rootDirIt->second[0];
        }
    }
    return "";
}

bool HttpRequestHandler::isAutoIndexEnabled(const std::string& uri) const
{
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator matchedLocation = _locInfo.end();
    std::string requestUri = uri;
    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator it = _locInfo.begin(); it != _locInfo.end(); ++it)
    {
        const std::string& locationUri = it->first;
        if (requestUri.find(locationUri) == 0 && 
            (matchedLocation == _locInfo.end() || locationUri.size() > matchedLocation->first.size()))
        {
            matchedLocation = it;
        }
    }
    const std::map<std::string, std::vector<std::string> >& locConfig = matchedLocation->second;
    std::map<std::string, std::vector<std::string> >::const_iterator autoIndexIt = locConfig.find("auto_index");
    if (autoIndexIt != locConfig.end() && !autoIndexIt->second.empty())
    {
        if (autoIndexIt->second[0] == "on")
        {
            return true;
        }
    }
    return false;
}

unsigned int HttpRequestHandler::getMaxBodyFromLoc(const std::string& uri) const
{
    std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator matchedLocation = _locInfo.end();
    std::string requestUri = uri;
    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator it = _locInfo.begin(); it != _locInfo.end(); ++it)
    {
        const std::string& locationUri = it->first;
        if (requestUri.find(locationUri) == 0 && 
            (matchedLocation == _locInfo.end() || locationUri.size() > matchedLocation->first.size()))
        {
            matchedLocation = it;
        }
    }
    if (matchedLocation == _locInfo.end())
    {
        return 1;
    }
    const std::map<std::string, std::vector<std::string> >& locConfig = matchedLocation->second;
    std::map<std::string, std::vector<std::string> >::const_iterator maxBodyIt = locConfig.find("max_body");
    if (maxBodyIt != locConfig.end() && !maxBodyIt->second.empty())
    {
        unsigned int maxBody = static_cast<unsigned int>(std::atoi(maxBodyIt->second[0].c_str()));
        return maxBody;
    }
    return 1;
}

std::string HttpRequestHandler::getFullPathFromLoc(const std::string& relativePath) const
{
    std::string rootDirectory = getRootDirectoryFromLoc(relativePath);
    if (!rootDirectory.empty())
    {
        return rootDirectory + relativePath;
    }
    return "";
}

bool HttpRequestHandler::isAutoIndexEnabledForUri(const std::string& uri) const
{
    return isAutoIndexEnabled(uri);
}

std::string HttpRequestHandler::getContentPath(const std::map<std::string, std::vector<std::string> >& config)
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = config.find("content_path");
    if (it != config.end() && !it->second.empty())
    {
        return it->second[0];
    }
    return "";
}

std::string HttpRequestHandler::getContPath() { return _contentPath; };
void HttpRequestHandler::setContentPath(const std::string& path) { this->_contentPath = path; };