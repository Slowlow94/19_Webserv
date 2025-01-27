#ifndef REQUESTRESPONSESTATE_HPP
# define REQUESTRESPONSESTATE_HPP

#pragma once

#include <cstddef>
# include <iostream>

#include "HttpRequestHandler.hpp"
#include "HttpResponseHandler.hpp"
#include "ServerHandler.hpp"

class HttpRequestHandler;
class HttpResponseHandler;
class ServerHandler;

class	RRState
{
	private:
		HttpRequestHandler				_request;
		HttpResponseHandler				_response;
		ServerHandler					_server;
		int								_clientSock;
		std::string						_readBuffer;
		std::string						_writeBuffer;
	
	public:
	//METHODS
	RRState();
	~RRState();

	//GETTER
	HttpRequestHandler&				getRequest();
	HttpResponseHandler&			getResponse();
	ServerHandler&					getServer();
	int&							getClientSock();
	std::string&					getWriteBuffer();
	const std::string&				getWriteBuffer() const;
	void							setWriteBuffer(const std::string& buffer);


	//SETTER
	void				setRequest(HttpRequestHandler& req);
	void				setResponse(HttpResponseHandler& res);
	void				setServer(const ServerHandler& server);
	void				setClientSock(const int& clientSock);
};

#endif
