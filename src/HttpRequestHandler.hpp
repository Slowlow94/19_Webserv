/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestHandler.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nechaara <nechaara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:10:05 by clegros           #+#    #+#             */
/*   Updated: 2025/01/20 01:58:01 by nechaara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTHANDLER_HPP
# define HTTPREQUESTHANDLER_HPP

# include "DirectoryHandler.hpp"
# include "configuration_file_parsing/server_config/ServerConfig.hpp"

# include <sys/socket.h>
# include <iostream>
# include <unistd.h>
# include <map>
# include <vector>
# include <sstream>
# include <string>
# include <fstream>
# include <sys/stat.h>
# include <cstring>
# include <algorithm>

class RRState;
class HttpResponseHandler;

class HttpRequestHandler
{
	private:
		int																			_clientSocket;
		std::string																	_method;
		std::string																	_path;
		std::string																	_httpVersion;
		std::map<std::string, std::string>											_headers;
		std::string																	_body;
		std::string 																_requestBuffer;
		bool																		_valid;

		std::vector<std::string>													_allowedMethods;
		std::vector<std::string>													_allowedPaths;
		std::string																	_allowedPath;
		std::string																	_contentPath;
		unsigned int																_maxBodySize;
		bool																		_autoIndex;
		std::string																	_rootDirectory;
		std::vector<std::string>													_cgiPath;
		LocationBlock																_locationBlocks;
		
		std::map<std::string, std::map<std::string, std::vector<std::string> > >	_locInfo;
		bool																		_isRequestComplete;
		DirectoryHandler															_handler;
		std::string																	_cookie;
		static std::string															extractBoundary(const std::string& contentType);
		
	public:
		HttpRequestHandler();
		~HttpRequestHandler();

		std::string																		trim(const std::string& str);
		bool																			endsWith(const std::string& str, const std::string& suffix);
		std::string																		getMimeType(const std::string& path);
		std::string																		createErrorPage(int statusCode, const std::string& message);
		std::string																		toString(size_t value);
		static std::string																readFile(const std::string& path);
		bool																			isMethodAllowed(const HttpRequestHandler& request, const std::string& method) const;
		void 																			appendToBuffer(const char* data, size_t length);
		void 																			clearBuffer();

		void																			setMethod(const std::string &m);
		void																			setPath(const std::string &p);
		void																			setHttpVersion(const std::string &h);
		void																			setHeader(const std::string &headerName, const std::string &headerValue);
		void																			setBody(const std::string &body);
		void																			setIsComplete(const bool& is);
		void																			setAllowedMethods(const std::vector<std::string>& methods);
		void																			setAllowedPaths(const std::vector<std::string>& paths);
		void																			setRootDirectory(const std::string& path);
		void																			setCgiPath(const std::vector<std::string>& cgiPath);
		void																			setClientSocket(const int& clientSock);

		std::string																		getMethod(void) const;
		std::string																		getPath(void) const;
		std::string																		getHttpVersion(void) const;
		std::string																		getHeader(const std::string &headerName) const;
		const std::map<std::string, std::string>&										getHeaders() const;
		std::string																		getBody() const;
		bool																			getIsComplete() const;
		std::string																		getRootDirectory() const;
		const std::vector<std::string>&													getAllowedMethods() const;
		const std::vector<std::string>&													getAllowedPaths() const;
		const std::vector<std::string>&													getCgiPath() const;
		const int&																		getClientSocket() const;
		std::string																		getCookie(const std::string& name) const;
		std::string& 																	getRequestBuffer();


// --------------------------------									

		void																			setLocInfo(const std::map<std::string, std::map<std::string, std::vector<std::string> > >& locInfo);
		void																			setIsValid(const bool& is);
		void																			setAllowedPath(const std::string& paths);
		void																			setAutoIndex(const bool& index);
		void																			setMaxBody(const unsigned int& max);

		bool																			getAutoIndex() const;
		const unsigned int&																getMaxBody() const;
		const std::string&																getAllowedPath() const;
		const std::map<std::string, std::map<std::string, std::vector<std::string> > >&	getLocInfo()const;
		std::map<std::string, std::vector<std::string> >								getLocInfoByUri(HttpRequestHandler request);
		bool																			getIsValid() const;

		std::vector<std::string>														getContentPathsFromLoc(const std::string& uri) const;
		std::string																		getRootDirectoryFromLoc(const std::string& uri) const;
		bool																			isAutoIndexEnabled(const std::string& uri) const;
		std::string																		getFullPathFromLoc(const std::string& relativePath) const;
		unsigned int																	getMaxBodyFromLoc(const std::string& uri) const;
		void																			setContentPath(const std::string& path);
		std::string																		getContPath();
		std::string 																	getContentPath(const std::map<std::string, std::vector<std::string> >& config);

		bool																			isAutoIndexEnabledForUri(const std::string& uri) const;
		void																			reset();
		LocationBlock*																	getLocationBlock(std::vector<LocationBlock*> locationBlocks) const;

// --------------------------------									

		HttpRequestHandler																handleRequest(const std::string& requestData, RRState& rrstate);
		static HttpRequestHandler														httpParsing(const std::string &buffer);
		void																			handleDirectoryRequest(RRState& rrstate, const std::string& path);
    	void																			handleFileUpload(RRState& rrstate, const std::string& requestData);
		HttpRequestHandler																handleConfig(HttpRequestHandler& request, std::vector<LocationBlock *> locationsBlock);
		std::string																		extractDir(std::string& requestPath);
		std::string																		removeExcessiveSlashes(std::string& path);

		void																			parseCookies(const std::string& cookieHeader);
		const std::map<std::string, std::string>&										getCookies() const;
		std::string																		trimCookies(const std::string& str);

};
std::ostream	&operator<<(std::ostream &out, const HttpRequestHandler &i);

#endif