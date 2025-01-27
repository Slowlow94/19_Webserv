#include "HttpRequestHandler.hpp"
#include "HttpResponseHandler.hpp"
#include "RequestResponseState.hpp"

void	HttpRequestHandler::handleDirectoryRequest(RRState& rrstate, const std::string& path)
{
	std::string				dirPath;
	std::string				content;
	std::vector<FileInfo>	files;

	dirPath	= rrstate.getRequest().getContPath() + path;
	files = _handler.getDirectoryListing(dirPath);
	content = _handler.generateDirectoryPage(path, files, dirPath);
	rrstate.getResponse().setStatusCode(200);
	rrstate.getResponse().setStatusMsg("OK");
	rrstate.getResponse().setHeader("Content-Type", "text/html");
	rrstate.getResponse().setHttpVersion("HTTP/1.1");
	rrstate.getResponse().setHeader("Content-Length", toString(content.length()));
	rrstate.getResponse().setBody(content);
}

void HttpRequestHandler::handleFileUpload(RRState& rrstate, const std::string& requestData)
{
    std::string contentType = getHeader("Content-Type");
    std::string filename;
    std::string fileContent = requestData;
    std::string uploadPath;
    std::string fullPath;

    unsigned int max = rrstate.getRequest().getMaxBodyFromLoc(rrstate.getRequest().getPath());
    if (requestData.length() > max)
    {
        setErrorResponse(rrstate, 413, "Payload Too Large");
        return;
    }
    if (contentType == "plain/text")
    {
        time_t now = time(0);
        std::ostringstream oss;
        oss << "upload_" << now << ".txt";
        filename = oss.str();
    }
    else if (contentType.find("multipart/form-data") != std::string::npos)
    {
        std::string boundary = extractBoundary(contentType);
        if (boundary.empty())
        {
            setErrorResponse(rrstate, 400, "Bad Request - Missing or invalid boundary in multipart form data");
            return;
        }
        std::string::size_type pos = requestData.find(boundary);
        if (pos == std::string::npos)
        {
            setErrorResponse(rrstate, 400, "Bad Request - Missing or invalid boundary in multipart form data");
            return;
        }
        pos = requestData.find("filename=\"", pos);
        if (pos == std::string::npos)
        {
            setErrorResponse(rrstate, 400, "Bad Request - Missing filename field from data");
            return;
        }

        pos += 10;
        std::string::size_type endPos = requestData.find("\"", pos);
        if (endPos == std::string::npos)
        {
            setErrorResponse(rrstate, 400, "Bad Request - Wrong filename from data");
            return;
        }
        filename = requestData.substr(pos, endPos - pos);
        pos = requestData.find("\r\n\r\n", endPos);
        if (pos == std::string::npos)
        {
            setErrorResponse(rrstate, 400, "Bad Request - Invalid filename format from data");
            return;
        }
        pos += 4;
        std::string::size_type contentEnd = requestData.find(boundary, pos);
        /*if (contentEnd == std::string::npos)
        {
            setErrorResponse(rrstate, 400, "Bad Request - Invalid file content ending from data");
            return;
        }*/

        contentEnd -= 2;
        fileContent = requestData.substr(pos, contentEnd - pos);
    }
    else
    {
        setErrorResponse(rrstate, 400, "Bad Request - Unsupported Content-Type: " + contentType);
            return;
    }
    uploadPath = rrstate.getRequest().getContPath() + rrstate.getRequest().getPath();
    if (rrstate.getResponse().isCgiRequest(rrstate, rrstate.getRequest().getPath()))
    {
        uploadPath = rrstate.getServer().getImagesPathCgi();
    }
    if (!_handler.isDirectory(uploadPath.c_str()))
    {
        if (!_handler.createDirectory(uploadPath.c_str()))
        {
            setErrorResponse(rrstate, 403, "Forbidden - Folder cannot be uploaded");
            return;
        }
    }
    fullPath = uploadPath + "/" + rrstate.getResponse().urlDecode(filename);
    std::ofstream file(fullPath.c_str(), std::ios::binary);
    if (!file)
    {
        setErrorResponse(rrstate, 403, "Forbidden - File cannot be uploaded");
        return;
    }
    file.write(fileContent.c_str(), fileContent.length());
    file.close();
    rrstate.getResponse().setStatusCode(303);
    rrstate.getResponse().setStatusMsg("Created");
    rrstate.getResponse().setHeader("Location", rrstate.getRequest().getPath());
    rrstate.getResponse().setHeader("Content-Type", "text/plain");
    rrstate.getResponse().setHttpVersion("HTTP/1.1");
    rrstate.getResponse().setHeader("Content-Length", "0");
}
