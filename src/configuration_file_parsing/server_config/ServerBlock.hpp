#ifndef SERVER_BLOCK_HPP
# define SERVER_BLOCK_HPP

#include <set>
#include <map>
#include <vector>
#include <iostream>
#include "ADirective.hpp"

struct s_server_params {
	std::string								_server_name;
	std::set<unsigned int>					_listen;
	std::map<unsigned int, std::string>		_error_pages_record;
	std::vector <std::string>				_declared_path;
};

#ifndef IS_LINUX
	#define IS_LINUX 0
#endif

#if defined(__linux__)
    #undef IS_LINUX
    #define IS_LINUX 1
#endif

class ServerBlock : public ADirective {
	private:
		s_server_params	_server_params;
		bool 			_listening_ports_set;
		bool			checkDeclaredPathOccurence(std::string path);
	public:
		ServerBlock(void);
		~ServerBlock();
		s_server_params getServerParams(void) const;
		
		// Setters
		void setServerName(std::string server_names);
		bool setListeningPort(std::set<unsigned int> listening_ports);
		void setErrorPagesRecord(std::map<unsigned int, std::string> error_pages_record);

		// Getter
		std::string							getServerName(void) const;
		std::set<unsigned int>				getListeningPort(void) const;
		bool								wasListeningPortSet(void) const;
		std::map<unsigned int, std::string>	getErrorPagesRecord(void) const;
};

std::ostream& operator<<(std::ostream& os, const ServerBlock *server_params);

#endif