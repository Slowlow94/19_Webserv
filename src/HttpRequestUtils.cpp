# include "HttpRequestHandler.hpp"
# include "RequestResponseState.hpp"

std::string HttpRequestHandler::extractDir(std::string& requestPath) {
    if (requestPath.empty()) {
        return "";
    }
    size_t pos = requestPath.find('/');
    if (pos == std::string::npos) {
        return "";
    }
    size_t dirStartPos = requestPath.find('/', pos + 1);
    if (dirStartPos == std::string::npos) {
        return "";
    }
    size_t dirEndPos = requestPath.find('/', dirStartPos + 1);
    if (dirEndPos != std::string::npos) {
        return requestPath.substr(dirStartPos, dirEndPos - dirStartPos);
    }
    return requestPath.substr(dirStartPos);
}

std::string HttpRequestHandler::removeExcessiveSlashes(std::string& path) {
    std::string result;
    bool was_last_slash = false;

    for (std::string::size_type i = 0; i < path.size(); ++i) {
        char c = path[i];

        if (c == '/') {
            if (!was_last_slash) {
                result += c;
                was_last_slash = true;
            }
        } else {
            result += c;
            was_last_slash = false;
        }
    }
    while (result.size() > 1 && result[result.size() - 1] == '/') {
        result.erase(result.size() - 1, 1);
    }

    return result;
}



LocationBlock* HttpRequestHandler::getLocationBlock(std::vector<LocationBlock*> locationBlocks) const
{
    std::string requestPath = this->getPath();
    LocationBlock* matchedBlock = NULL;
    size_t longestMatch = 0;

    std::vector<LocationBlock*>::const_iterator it = locationBlocks.begin();
    for (; it != locationBlocks.end(); ++it) {
        LocationBlock* block = *it;
        const std::string& locationUri = block->getLocationParams()._uri;
        if (requestPath == locationUri)
            return (block);
    }
    it = locationBlocks.begin();
    for (; it != locationBlocks.end(); ++it) {
        LocationBlock* block = *it;
        const std::string& locationUri = block->getLocationParams()._uri;
        if (locationUri == "/" && requestPath.length() > locationUri.length() &&
            it == locationBlocks.begin()) {
            continue;
        }
        if ((requestPath.find(locationUri + "/") == 0) || requestPath == locationUri) {
            size_t matchLength = locationUri.length();
            if (matchLength > longestMatch) {
                longestMatch = matchLength;
                matchedBlock = block;
            }
        }
    }
    return (matchedBlock);
}

std::string HttpRequestHandler::trim(const std::string& str)
{
	size_t	first;
	size_t	last;

    first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
		return "";
    last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

bool HttpRequestHandler::endsWith(const std::string& str, const std::string& suffix)
{
    if (str.length() < suffix.length())
		return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool HttpRequestHandler::isMethodAllowed(const HttpRequestHandler& request, const std::string& method) const
{
	std::vector<std::string> alwdMet = request.getAllowedMethods();
	for (size_t i = 0; i < alwdMet.size(); i++)
	{
			if (alwdMet[i] == method)
			{
					return true;
			}
	}
	return false;
}

std::string HttpRequestHandler::toString(size_t value)
{
    std::ostringstream	oss;
    oss << value;
    return oss.str();
}

std::string HttpRequestHandler::getMimeType(const std::string& path)
{
    if (endsWith(path, ".html")) return "text/html";
    if (endsWith(path, ".css")) return "text/css";
    if (endsWith(path, ".js")) return "application/javascript";
	if (endsWith(path, ".json")) return "application/json";
    if (endsWith(path, ".png")) return "image/png";
    if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) return "image/jpeg";
    if (endsWith(path, ".gif")) return "image/gif";
    if (endsWith(path, ".pdf")) return "application/pdf";
    return "text/plain";
}

std::string HttpRequestHandler::createErrorPage(int statusCode, const std::string& message)
{
    std::ostringstream	oss;
    oss << "<html><head><title>" << statusCode << " - " << message << "</title></head>";
    oss << "<body><h1>" << statusCode << " - " << message << "</h1></body></html>";
    return oss.str();
}

std::ostream& operator<<(std::ostream& out, const HttpRequestHandler& handler)
{
	out << "\n---------------------------REQUEST---------------------------------\n";
    out << handler.getMethod() << " " << handler.getPath() << " " << handler.getHttpVersion() << "\n";
    const std::map<std::string, std::string>&	headers = handler.getHeaders();
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); 
         it != headers.end(); ++it)
	{
        out << it->first << ": " << it->second << "\n";
    }
	out << handler.getAutoIndex() << "\n";
	out << "\nmaxBody: \n";
	out << handler.getMaxBody() << "\n";
	out << "\n---------------------------REQUEST---------------------------------\n";
    return out;
}

std::string removeRoot(const std::string& str, const std::string& root) {
    std::string result = str;
    size_t pos = result.find(root);
    if (pos != std::string::npos) {
        result.erase(pos, root.length());
    }
    return result;
}

std::string HttpRequestHandler::readFile(const std::string& path) {
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return ("");
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        return (std::string(buffer.begin(), buffer.end()));
    } else {
        return ("");
    }
}


std::string HttpRequestHandler::extractBoundary(const std::string& contentType)
{
    size_t	pos;
	pos = contentType.find("boundary=");
    if (pos != std::string::npos)
	{
        return "--" + contentType.substr(pos + 9);
    }
    return "";
}

void 																			HttpRequestHandler::setMethod(const std::string& m) { _method = m; }
void 																			HttpRequestHandler::setPath(const std::string& p) { _path = p; }
void 																			HttpRequestHandler::setHttpVersion(const std::string& h) { _httpVersion = h; }
void 																			HttpRequestHandler::setHeader(const std::string& name, const std::string& value) { _headers[trim(name)] = trim(value); }
void 																			HttpRequestHandler::setBody(const std::string& b) { _body = b; }
void 																			HttpRequestHandler::setIsComplete(const bool& is) { _isRequestComplete = is; }
void 																			HttpRequestHandler::setAllowedMethods(const std::vector<std::string>& methods) { _allowedMethods = methods; }
void 																			HttpRequestHandler::setAllowedPaths(const std::vector<std::string>& paths) { _allowedPaths = paths; }
void 																			HttpRequestHandler::setAllowedPath(const std::string& path) { _allowedPath = path; }
void 																			HttpRequestHandler::setRootDirectory(const std::string& path) { _rootDirectory = path; }
void 																			HttpRequestHandler::setCgiPath(const std::vector<std::string>& cgiPath) { _cgiPath = cgiPath; }
void 																			HttpRequestHandler::setClientSocket(const int& clientSock) {_clientSocket = clientSock;}
																			
void 																			HttpRequestHandler::setLocInfo(const std::map<std::string, std::map<std::string, std::vector<std::string> > >& locInfo) { _locInfo = locInfo; }
void 																			HttpRequestHandler::setIsValid(const bool& val) { _valid = val; }
void 																			HttpRequestHandler::setAutoIndex(const bool& index) { _autoIndex = index; }
void 																			HttpRequestHandler::setMaxBody(const unsigned int& max) { _maxBodySize = max; }

std::string 																	HttpRequestHandler::getMethod() const { return _method; }
std::string 																	HttpRequestHandler::getPath() const { return _path; }
std::string 																	HttpRequestHandler::getHttpVersion() const { return _httpVersion; }
std::string 																	HttpRequestHandler::getBody() const { return _body; }
std::string 																	HttpRequestHandler::getHeader(const std::string& name) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(name);
    return it != _headers.end() ? it->second : "";
}
const std::map<std::string, std::string>&										HttpRequestHandler::getHeaders() const { return _headers; }
bool    																		HttpRequestHandler::getIsComplete() const { return _isRequestComplete; }
std::string																		HttpRequestHandler::getRootDirectory() const { return _rootDirectory; }
const std::vector<std::string>& 												HttpRequestHandler::getAllowedMethods() const { return _allowedMethods; }
const std::vector<std::string>& 												HttpRequestHandler::getAllowedPaths() const { return _allowedPaths; }
const std::string&																HttpRequestHandler::getAllowedPath() const { return _allowedPath; }
const std::vector<std::string>&													HttpRequestHandler::getCgiPath() const { return _cgiPath;}
const int&																		HttpRequestHandler::getClientSocket() const {return _clientSocket; };
const std::map<std::string, std::map<std::string, std::vector<std::string> > >&	HttpRequestHandler::getLocInfo() const { return _locInfo; }
bool																			HttpRequestHandler::getIsValid() const { return _valid; }
bool																			HttpRequestHandler::getAutoIndex() const { return _autoIndex; }
const unsigned int&																HttpRequestHandler::getMaxBody() const { return _maxBodySize; }
std::string                                                                     HttpRequestHandler::getCookie(const std::string& name) const
{
    std::string cookies = getHeader("Cookie");
    size_t pos = cookies.find(name + "=");
    if (pos != std::string::npos) {
        size_t start = pos + name.length() + 1;
        size_t end = cookies.find(";", start);
        if (end == std::string::npos)
            end = cookies.length();
        return cookies.substr(start, end - start);
    }
    return "";
}

// BUFFER // 
void HttpRequestHandler::appendToBuffer(const char* data, size_t length) {
    _requestBuffer.append(data, length);
}

std::string& HttpRequestHandler::getRequestBuffer() {
    return _requestBuffer;
}

void HttpRequestHandler::clearBuffer() {
    _requestBuffer.clear();
}