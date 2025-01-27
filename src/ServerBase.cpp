
#include "ServerBase.hpp"
#include "ServerHandler.hpp"
#include "RequestResponseState.hpp"
#include "HttpResponseHandler.hpp"
#include "HttpRequestHandler.hpp"
#include "ErrorHandler.hpp"

//METHODS
ServerBase::ServerBase() : _maxSock(0)
{
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
}

ServerBase::~ServerBase()
{
	for (std::vector<ServerHandler>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		int sock = it->getSock();
		FD_CLR(sock, &_readfds);
		FD_CLR(sock, &_writefds);
		close(sock);
	}
}

/////////// GETTER ///////////////
fd_set&	ServerBase::getReadfds() { return _readfds; }
fd_set&	ServerBase::getWritefds() { return _writefds; }
std::vector<ServerHandler>	ServerBase::getServers() { return _servers; }

////////// PUBLIC /////////////
void	ServerBase::addPortAndServers(std::map <size_t, ServerConfig *> AllServersConfig)
{
	std::string ImagesPath;
	for (std::map<size_t, ServerConfig *>::iterator it = AllServersConfig.begin(); it != AllServersConfig.end(); it++)
	{
		s_server_params server_params = it->second->getServerHeader()->getServerParams();
		std::vector<LocationBlock *> directives = it->second->getDirectives();
		for (std::vector<LocationBlock *>::iterator it = directives.begin(); it != directives.end(); it++) {
			ImagesPath = (*it)->getUriDependance();
			if (!ImagesPath.empty())
				break;
		}
		
		std::string	server_name = it->second->getServerHeader()->getServerName();
		std::map <unsigned int, std::string> error_pages = it->second->getServerHeader()->getErrorPagesRecord();
		for (std::set<unsigned int>::iterator it = server_params._listen.begin(); it != server_params._listen.end(); it++)
		{
			ServerHandler NewServer;
			NewServer.setLocations(directives);
			NewServer.setServerName(server_name);
			NewServer.setImagesPathCgi(ImagesPath);
			NewServer.InitializeServerSocket(*it, 3);
			NewServer.setErrorPages(error_pages);
			_logger.info("Server Created FD: " + toStrInt(NewServer.getSock()) + " ~ Port: " +  toStrInt(NewServer.getPort())); 
			FD_SET(NewServer.getSock(), &getReadfds());
			this->_maxSock = std::max(this->_maxSock, NewServer.getSock());
			_servers.push_back(NewServer);
		}
	}
}

void	ServerBase::acceptConnection(ServerHandler Server)
{
    int newSocket = accept(Server.getSock(), Server.getAddress(), &Server.getAddrlen());
    if (newSocket < 0)
	{
		throw ServerBaseError("Accept failed", __FUNCTION__, __LINE__);
	}
	FD_SET(newSocket, &_readfds);
	RRState NewConnectionState;
	NewConnectionState.setServer(Server);
	_clientSockets.insert(std::make_pair(newSocket, NewConnectionState));
	if (newSocket > _maxSock)
		_maxSock = newSocket;
}

void ServerBase::processClientConnections()
{
    fd_set cpyReadFds, cpyWriteFds;
    std::map<int, std::string> clientBuffers;

    while (true)
    {
        cpyReadFds = _readfds;
        cpyWriteFds = _writefds;
        std::vector<int> clientToRemove;

        if (select(_maxSock + 1, &cpyReadFds, &cpyWriteFds, NULL, NULL) < 0)
            throw ServerBaseError("Select failed", __FUNCTION__, __LINE__);
        for (unsigned long i = 0; i < this->_servers.size(); i++)
        {
            int serverSocket = this->_servers[i].getSock();
            if (FD_ISSET(serverSocket, &cpyReadFds))
                acceptConnection(this->_servers[i]);
        }
        for (std::map<int, RRState>::iterator it = _clientSockets.begin(); it != _clientSockets.end(); it++)
        {
            int clientSock = it->first;

            if (FD_ISSET(clientSock, &cpyReadFds))
            {
                char buffer[1024];
                int bytesRead = recv(clientSock, buffer, sizeof(buffer) - 1, 0);

                if (bytesRead <= 0)
                {
                    close(clientSock);
                    FD_CLR(clientSock, &_readfds);
                    FD_CLR(clientSock, &_writefds);
                    clientToRemove.push_back(clientSock);
                    continue;
                }

                buffer[bytesRead] = '\0';
                clientBuffers[clientSock] += std::string(buffer, bytesRead);

                HttpRequestHandler request = request.handleRequest(clientBuffers[clientSock], it->second);
                it->second.setRequest(request);
                // std::cout << request << std::endl;
                if (request.getIsComplete())
                {
                    clientBuffers[clientSock].clear();
                    FD_CLR(clientSock, &_readfds);
                    FD_SET(clientSock, &cpyWriteFds);
                }
            }

            if (FD_ISSET(clientSock, &cpyWriteFds))
            {
                HttpResponseHandler response = it->second.getResponse();
                response.handleResponse(it->second);
                it->second.setResponse(response);
                it->second.setWriteBuffer(response.getAll());

                std::string& responseBuffer = it->second.getWriteBuffer();
                if (!responseBuffer.empty())
                {
                    ssize_t bytesSent = send(clientSock, responseBuffer.c_str(), responseBuffer.size(), 0);
                    if (bytesSent <= 0)
                    {
                        close(clientSock);
                        FD_CLR(clientSock, &_writefds);
                        clientToRemove.push_back(clientSock);
                        continue;
                    }
                    responseBuffer.erase(0, bytesSent);
                }
                // std::cout << response << std::endl;
                if (responseBuffer.empty())
                {
                    close(clientSock);
                    FD_CLR(clientSock, &_writefds);
                    clientToRemove.push_back(clientSock);
                }
            }
        }

        for (size_t i = 0; i < clientToRemove.size(); i++)
        {
            int clientSock = clientToRemove[i];
            _clientSockets.erase(clientSock);
            clientBuffers.erase(clientSock);
        }
    }
}
