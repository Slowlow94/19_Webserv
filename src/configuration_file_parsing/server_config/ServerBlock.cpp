#include "ServerBlock.hpp"

ServerBlock::ServerBlock(void) {
	_listening_ports_set = false;
}

ServerBlock::~ServerBlock() {}

// **************************************************************************************

s_server_params ServerBlock::getServerParams(void) const {
	return (this->_server_params);
}

void ServerBlock::setServerName(std::string server_names) {
	this->_server_params._server_name = server_names;
}

bool ServerBlock::setListeningPort(std::set<unsigned int> listening_ports) {
	if (!_listening_ports_set) {
		this->_server_params._listen = listening_ports;
		_listening_ports_set = true;
	} else {
		for (std::set<unsigned int>::iterator it = listening_ports.begin(); it != listening_ports.end(); it++) {
			this->_server_params._listen.insert(*it);
		}
	}
	return (true);
}

void ServerBlock::setErrorPagesRecord(std::map<unsigned int, std::string> error_pages_record) {
	unsigned int error_value;
	std::pair<unsigned int, std::string> current_pair;
	
	for (std::map<unsigned int, std::string>::iterator it = error_pages_record.begin(); it != error_pages_record.end(); it++) {
		current_pair = (*it);
		error_value = current_pair.first;
		this->_server_params._error_pages_record.erase(error_value);
		this->_server_params._error_pages_record.insert(*it);
	};
}

bool ServerBlock::checkDeclaredPathOccurence(std::string path) {
	for (std::vector<std::string>::iterator it = _server_params._declared_path.begin(); it != _server_params._declared_path.end(); it++) {
		if ((*it) == path)
			return (false);
	}
	return (true);
}

std::string ServerBlock::getServerName(void) const {
	return (this->_server_params._server_name);
}

std::set <unsigned int> ServerBlock::getListeningPort(void) const {
	return (this->_server_params._listen);
}

bool ServerBlock::wasListeningPortSet(void) const {
	return (this->_listening_ports_set);
}

std::map<unsigned int, std::string> ServerBlock::getErrorPagesRecord(void) const {
	return (this->_server_params._error_pages_record);
} 

std::ostream& operator<<(std::ostream& os, const ServerBlock *params) {
	const std::set<unsigned int> listening_ports = params->getListeningPort();
	const std::string server_names = params->getServerName();
	const std::map<unsigned int, std::string> error_pages_record = params->getErrorPagesRecord();

	std::cout << "\n\n" << "SERVER BLOCK" << "\n\n";
	static_cast<const ADirective *>(params)->printAttributes(os);
	os << server_names << "\n";
	for (std::set<unsigned int>::iterator it = listening_ports.begin(); it != listening_ports.end(); it++)
		os << "Listening ports : " << (*it) << "\n"; 
	for (std::map<unsigned int, std::string>::const_iterator it = error_pages_record.begin(); it != error_pages_record.end(); it++) {
		if (it == error_pages_record.begin())
			os << "Error pages redefinition" << "\n";
		os << "Port : " << (*it).first << " | Path : " << (*it).second << std::endl;
	}
	return (os);
}