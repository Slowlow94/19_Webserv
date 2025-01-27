/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nechaara <nechaara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:07:27 by clegros           #+#    #+#             */
/*   Updated: 2025/01/22 00:44:29 by nechaara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSEHANDLER_HPP
# define HTTPRESPONSEHANDLER_HPP

# include <iostream>
# include <sstream>
# include <fstream>
# include <map>
# include <vector>
# include <sys/stat.h>
# include <sys/socket.h>
# include <string>
# include <cstdlib>
# include <cstdio>
# include <cstring>
# include <stdexcept>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include "configuration_file_parsing/server_config/ADirective.hpp"

class RRState;
class LocationBlock;

class HttpResponseHandler
{
	public:
		void									setHttpVersion(std::string version);
		void									setStatusCode(int code);
		void									setStatusMsg(std::string message);
		void									setHeader(const std::string &headerName, const std::string &headerValue);
		void									setBody(std::string body);
		void    								setQuery(std::string query);

		std::string								getHttpVersion() const;
		int										getStatusCode() const;
		std::string								getStatusMsg() const;
		std::string								getHeader(const std::string &headerName) const;
		std::map<std::string, std::string>		getHeaders() const;
		std::string								getBody() const;
		std::string								getAll() const;
		std::string								getQuery() const;
		std::string								getPathOfFile(RRState& rrstate);
		
		HttpResponseHandler						handlePath(RRState& rrstate);
		HttpResponseHandler						handleResponse(RRState& rrstate);
		std::string								urlDecode(const std::string& url);
		HttpResponseHandler						errorHandler(RRState &rrstate, unsigned int error_code, std::string message);
		std::pair<std::string, e_data_reach>	checkAvailableRessource(std::string& file_path);
		bool									isValidHost(RRState& rrstate);
		HttpResponseHandler						handleGet(RRState& rrstate);
		bool									isCgiRequest(RRState& rrstate, const std::string& path);

		std::string								_httpVersion;
		int										_code;
		std::string								_status;
		std::map<std::string, std::string>		_headers;
		std::string								_body;
		std::string								_query;
};
void				setErrorResponse(RRState& rrstate, int statusCode, const std::string& statusMsg);
std::ostream		&operator<<(std::ostream &out, const HttpResponseHandler &i);

#endif