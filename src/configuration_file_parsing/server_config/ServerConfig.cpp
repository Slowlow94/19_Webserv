#include "ServerConfig.hpp"

ServerConfig::ServerConfig(void) {
	this->_server_header = NULL;
}

ServerConfig::~ServerConfig() {
	for (std::size_t i = 0; i < _directives.size(); i++) {
		delete _directives[i];
	}
	if (_server_header)
		delete _server_header;
}

std::vector<LocationBlock *> ServerConfig::getDirectives(void) const {
	return (this->_directives);
}

void ServerConfig::setDirective(LocationBlock *new_directive) {
	this->_directives.push_back(new_directive);
}

void ServerConfig::setServerHeader(ServerBlock *server_header) {
	this->_server_header = server_header;
}

ServerBlock* ServerConfig::getServerHeader(void) const {
	return (this->_server_header);
}
