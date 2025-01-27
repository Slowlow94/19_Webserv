#ifndef SERVERBASE_HPP
# define SERVERBASE_HPP

#include "RequestResponseState.hpp"
#include "ServerHandler.hpp"
#include "configuration_file_parsing/server_config/ServerConfig.hpp"
#include "configuration_file_parsing/server_config/LocationBlock.hpp"

#include <iostream>
#include <sys/select.h>
#include <vector>
#include <map>
#include <algorithm>
#include <cstring>

class	ServerBase
{
	private:
		int							_maxSock;
		std::map<int, RRState>		_clientSockets;
		std::vector<ServerHandler>	_servers;
		fd_set 						_readfds, _writefds;
		Logger						_logger;

	public:
		//METHODS
		ServerBase();
		~ServerBase();
		fd_set&								getReadfds();
		fd_set&								getWritefds();
		std::vector<ServerHandler>			getServers();

		void								processClientConnections();
		void								acceptConnection(ServerHandler	Server);
		void								addPortAndServers(std::map <size_t, ServerConfig *> AllServersConfig);
};

#endif
