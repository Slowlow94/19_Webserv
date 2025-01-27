#include "HttpRequestHandler.hpp"

HttpRequestHandler HttpRequestHandler::httpParsing(const std::string& buffer)
{
    HttpRequestHandler	request;
    std::istringstream	stream(buffer);
    std::string			line;
	std::string			name;
	std::string			value;
	std::string			body;
    std::string         unsanitized_path;
    size_t              methodEnd;
	size_t				pathStart;
	size_t				pathEnd;
	size_t				colonPos;
    
    if (std::getline(stream, line))
	{
        line = request.trim(line);
        methodEnd = line.find(' ');
        if (methodEnd != std::string::npos)
		{
            request.setMethod(line.substr(0, methodEnd));
            pathStart = methodEnd + 1;
            pathEnd = line.find(' ', pathStart);
            if (pathEnd != std::string::npos)
			{
                unsanitized_path = line.substr(pathStart, pathEnd - pathStart);
                request.setPath(request.removeExcessiveSlashes(unsanitized_path));
                request.setHttpVersion(line.substr(pathEnd + 1));
            }
        }
    }
    while (std::getline(stream, line) && !line.empty() && line != "\r")
	{
        line = request.trim(line);
        colonPos = line.find(':');
        if (colonPos != std::string::npos)
		{
            name = line.substr(0, colonPos);
            value = line.substr(colonPos + 1);
            request.setHeader(name, value);
        }
    }
    while (std::getline(stream, line))
	{
        body += line + "\n";
    }
    request.setBody(request.trim(body));
    return request;
}