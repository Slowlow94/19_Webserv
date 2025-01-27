#ifndef SOCKETHANDLER_HPP
# define SOCKETHANDLER_HPP

#include "configuration_file_parsing/server_config/ServerConfig.hpp"
#include "Logger.hpp"
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

class ErrorHandler;

class ServerHandler
{
	private:
		struct sockaddr_in					_address;
		socklen_t							_addrlen;
		int									_sock;
		int									_port;
		std::vector<LocationBlock *>		_locations;
		std::string							_server_name;
		std::map<unsigned int, std::string>	_errorPagesRecord;
		std::string							_imagesPath;

	public:
		// METHODS
		ServerHandler();
		~ServerHandler();
		int&							getSock();
		int&							getPort();
		struct sockaddr*				getAddress();
		struct sockaddr_in				getAdd();
		socklen_t&						getAddrlen();
		std::vector<LocationBlock *>&	getLocations();
		std::string						getServerName(void) const;
		std::string						getImagesPathCgi(void);

		void				setLocations(std::vector<LocationBlock *>& locations);
		void				setServerName(std::string server_name);
		void				setImagesPathCgi(std::string ImagesPath);


		void				InitializeServerSocket(int port, const int backlog);
		int					createSocket();
		void				setSocketOptions(int sock);
		void				bindSocket(int port);
		void				listenSocket(int sock, int backlog);
		void				setNonblocking(int sock);
		void				setErrorPages(std::map<unsigned int, std::string> error_map);

		std::map<unsigned int, std::string>	getErrors() const;
};

#endif