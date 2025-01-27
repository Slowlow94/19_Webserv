#include "ServerHandler.hpp"
#include "ErrorHandler.hpp"
#include "Logger.hpp"

// METHODS //
ServerHandler::ServerHandler() : _addrlen(sizeof(_address)){}
ServerHandler::~ServerHandler() {}

// GETTER //
int&                          ServerHandler::getSock() { return this->_sock; }
int&                          ServerHandler::getPort() { return this->_port; }
struct sockaddr*              ServerHandler::getAddress() { return reinterpret_cast<struct sockaddr*>(&_address); }
struct sockaddr_in            ServerHandler::getAdd() { return this->_address; } 
socklen_t&	                  ServerHandler::getAddrlen() { return _addrlen; }
std::vector<LocationBlock *>&	ServerHandler::getLocations() { return this->_locations; }
std::string                   ServerHandler::getServerName(void) const { return this->_server_name; }
std::string						        ServerHandler::getImagesPathCgi(void) { return this->_imagesPath; }

void  ServerHandler::setServerName(std::string server_name) { this->_server_name = server_name; }
void  ServerHandler::setLocations(std::vector<LocationBlock *>& locations) { this->_locations = locations; }
void  ServerHandler::setImagesPathCgi(std::string imagesPath)  {this->_imagesPath = imagesPath;}

void ServerHandler::setErrorPages(std::map <unsigned int, std::string> error_pages) {
  this->_errorPagesRecord = error_pages;
}

void ServerHandler::InitializeServerSocket(int port, const int backlog)
{
	this->_port = port;
  this->_sock = createSocket();

  setSocketOptions(this->_sock);
  bindSocket(this->_port);
  setNonblocking(this->_sock);
  listenSocket(this->_sock, backlog);
}

int ServerHandler::createSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
		throw ServerHandlerError("Creation of socket failed", __FUNCTION__, __LINE__);
    return sock;
}

void ServerHandler::setSocketOptions(int sock)
{
  int opt = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
		throw ServerHandlerError("setsockopt failed with SO_REUSEADDR", __FUNCTION__, __LINE__);
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (char*)&opt, sizeof(opt)) < 0)
		throw ServerHandlerError("setsockopt failed with SO_REUSEPORT", __FUNCTION__, __LINE__);
}

void ServerHandler::bindSocket(int port)
{
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(port);
	if (bind(this->_sock, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0)
		throw ServerHandlerError("bind failed", __FUNCTION__, __LINE__);
}

void ServerHandler::listenSocket(int sock, int backlog)
{
    if (listen(sock, backlog) < 0)
		throw ServerHandlerError("listen failed", __FUNCTION__, __LINE__);
}

void ServerHandler::setNonblocking(int sock)
{
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1)
		  throw ServerHandlerError("fcntl failed : unable to get flags", __FUNCTION__, __LINE__);
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1)
		  throw ServerHandlerError("fcntl failed: unable to set non-blocking", __FUNCTION__, __LINE__);
}

std::map<unsigned int, std::string> ServerHandler::getErrors() const { return _errorPagesRecord; }