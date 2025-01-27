#include "ConfigParser.hpp"
#include "ConfigException.hpp"

ConfigParser* ConfigParser::_instance = NULL;

/**
 * @brief Construct a new Config Parser:: Config Parser object
 * Initialize vectors of allowed tokens as well as unrepeatable
 * tokens in the context of either locations block or server blocks
 * @param init_path The config file path
 */
ConfigParser::ConfigParser(const std::string init_path) {
	std::ifstream configuration_input_file;

	initializeVector(_c_params, C_TOKENS);
	initializeVector(_l_params, L_BLOCK_TOKENS);
	initializeVector(_s_params, S_BLOCK_TOKENS);
	initializeVector(_non_repeat_tokens_l, L_NO_RPT_TOKENS);
	initializeVector(_non_repeat_tokens_s, S_NO_RPT_TOKENS);
	if (init_path.empty() || !endsWith(init_path, FILE_EXT))
		throw ConfigParserError(BAD_CONFIG_FILE, __FUNCTION__, __LINE__);
	configuration_input_file.open(init_path.c_str());
	if (!configuration_input_file.is_open())
		throw ConfigParserError(BAD_CONFIG_FILE, __FUNCTION__, __LINE__);
	parseConfigurationFile(configuration_input_file);
}

ConfigParser::~ConfigParser() {
	for (std::size_t i = 0; i < _servers_config.size(); i++) {
		_logger.warn("Disabling server " + toStrInt(i) + ".");
		delete _servers_config[i];
	}
}

ConfigParser* ConfigParser::getInstance(const std::string init_path = "") {
	if (!_instance)
		_instance = new ConfigParser(init_path);
	return (_instance);
}

ServerConfig* ConfigParser::getServerConfig(unsigned int id) const {
	std::map <size_t, ServerConfig *>::const_iterator it;

	it = this->_servers_config.find(id);
	if (it != this->_servers_config.end()) {
		return (it->second);
	}
	throw ConfigParserError(NO_SERVER_CONFIG, __FUNCTION__, __LINE__);
}

std::map <size_t, ServerConfig *> ConfigParser::getAllServerConfig(void) const {
	return (this->_servers_config);
}

void ConfigParser::setServerConfig(size_t server_id, ServerConfig *current_server) {
	this->_servers_config.insert(std::make_pair(server_id, current_server));
}

bool ConfigParser::CgiPathChecker(LocationBlock *l_block) {
	std::string full_path;
	std::string root_path;
	std::string partial_cgi_path;

	partial_cgi_path = l_block->getCgiPath();
	root_path = l_block->getContentPath();
	if (root_path[root_path.size() - 1] == '/')
		full_path = root_path + partial_cgi_path;
	else
		full_path = root_path + '/' + partial_cgi_path;
	_validator.setPath(full_path);
	if (_validator.exists() && _validator.isFile() && _validator.isExecutable() && _validator.isReadable()) {
		l_block->setCgiPath(full_path);
		return (true);
	}
	return (false);
}

bool ConfigParser::checkDependsOn(ServerConfig *current_config) {
	std::vector <LocationBlock *> all_directives = current_config->getDirectives();
	std::vector <LocationBlock *>::iterator main_it;
	std::vector <LocationBlock *>::iterator second_it;
	bool found_match = false;
	
	main_it = all_directives.begin();
	for (;main_it != all_directives.end(); main_it++) {
		if ((*main_it)->getUriDependance() == (*main_it)->getUri()) {
			_logger.critical("URI cannot depend on itself");
			return (false);
		}
		else if ((*main_it)->getUriDependance().empty())
			continue;
		else {
			second_it = all_directives.begin();
			for (;second_it != all_directives.end(); second_it++) {
				if (main_it == second_it)
					continue;
				else if ((*main_it)->getUriDependance() == (*second_it)->getUri()) {
					found_match = true;
					(*main_it)->setUriDependance((*second_it)->getContentPath());
					(*main_it)->setRawUriDependence((*second_it)->getUri());
				}
			}
			if (!found_match)
				return (false);
		}
	}
	return (true);
}

bool ConfigParser::checkAlias(ServerConfig *current_config) {
	std::vector <LocationBlock *> all_directives = current_config->getDirectives();
	std::vector <LocationBlock *>::iterator main_it;
	std::vector <LocationBlock *>::iterator second_it;
	bool found_match = false;
	
	main_it = all_directives.begin();
	for (;main_it != all_directives.end(); main_it++) {
		if ((*main_it)->getAlias() == (*main_it)->getUri()) {
			_logger.critical("Alias cannot be redirected to itself");
			return (false);
		}
		else if ((*main_it)->getAlias().empty())
			continue;
		else {
			second_it = all_directives.begin();
			for (;second_it != all_directives.end(); second_it++) {
				if (main_it == second_it)
					continue;
				else if ((*main_it)->getAlias() == (*second_it)->getUri()) {
					found_match = true;
					(*main_it)->setContentPath((*second_it)->getContentPath());
					(*main_it)->getIndex().clear();
					(*main_it)->setIndex((*second_it)->getIndex());
				}
			}
			if (!found_match)
				return (false);
		}
	}
	return (true);
}

void ConfigParser::validateCgiPaths(ServerConfig* serverConfig) {
	std::map<std::string, LocationBlock*> cgiPaths;

	std::vector<LocationBlock*> directives = serverConfig->getDirectives();
	std::vector<LocationBlock*>::iterator it;

	it = directives.begin();
	for (; it != directives.end(); ++it) {
		if ((*it)->isCgiAllowed()) {
			cgiPaths.insert(std::make_pair((*it)->getUri(), *it));
		}
	}
	for (it = directives.begin(); it != directives.end(); ++it) {
		std::string uri = (*it)->getUri();
		std::map<std::string, LocationBlock*>::iterator map_it;
		for (map_it = cgiPaths.begin(); map_it != cgiPaths.end(); ++map_it) {
			std::string cgiUri = map_it->first;
			if (uri.find(cgiUri) == 0 && uri.length() > cgiUri.length() && uri[cgiUri.length()] == '/') {
				throw ConfigParserError(CGI_PATH_HAS_SUBROUTE, __FUNCTION__, __LINE__, -1);
			}
		}
	}
}

void ConfigParser::finalizeServerBlock(ServerBlock *directive, ServerConfig *serv_conf , size_t server_id) {
	_logger.info("Finalizing server " + toStrInt(server_id + 1) + " parsing");
	if (!directive->wasListeningPortSet())
		throw ConfigParserError(PORT_NOT_SET, __FUNCTION__, __LINE__, -1);
	if (directive->getServerName().empty())
		directive->setServerName("localhost");
	if (!checkDependsOn(serv_conf))
		throw ConfigParserError(DEPENDS_ON_NO_MATCH, __FUNCTION__, __LINE__, -1);
	if (!checkAlias(serv_conf))
		throw ConfigParserError(ALIAS_NO_MATCH, __FUNCTION__, __LINE__, -1);
	if (directive->getRoot().empty())
		_logger.warn("Relying on root definition for each location directive - not recommanded");
	if (serv_conf->getDirectives().empty())
		_logger.warn("No directive set up for the current server");
	validateCgiPaths(serv_conf);
	std::vector<LocationBlock*> directives = serv_conf->getDirectives();
	if (!distinctUri(serv_conf))
		throw ConfigParserError(DOUBLE_LOCATION_URI, __FUNCTION__, __LINE__, -1);
}

void ConfigParser::finalizeLocationBlock(LocationBlock *directive, ServerBlock *server_config, std::string uri, size_t line) {
	std::string server_root = server_config->getRoot();
	std::string location_root = directive->getRoot();
	std::string root;
	std::set<std::string> default_index;

	if (location_root.empty() && server_root.empty())
		throw ConfigParserError(NO_ROOT_DEFINITION, __FUNCTION__, __LINE__, line);
	if (!server_root.empty() && location_root.empty())
		directive->setRoot(server_root);
	root = directive->getRoot();
	if (!directive->isContentPathModified()) {
		if (!directive->setContentPath(removeExcessiveSlashes(root + uri)))
			throw ConfigParserError(BAD_URI, __FUNCTION__, __LINE__, line);
	}
	if (!directive->setUri(removeExcessiveSlashes(uri), removeExcessiveSlashes(root)))
		throw ConfigParserError(BAD_URI, __FUNCTION__, __LINE__, line);
	if (uri.find("..") != std::string::npos)
        throw ConfigParserError(BAD_URI, __FUNCTION__, __LINE__, line);
 	if (directive->getIndex().empty() && !server_config->getIndex().empty())
		directive->setIndex(server_config->getIndex());
	if (!directive->getCgiPath().empty()) {
		if (!CgiPathChecker(directive))
			throw ConfigParserError(BAD_CGI_PATH, __FUNCTION__, __LINE__, line);
	}
	if (!directive->hasAutoIndexModified())
		directive->setAutoIndex(server_config->getAutoIndex());
	if (!directive->hasClientMaxBodySizeModified())
		directive->setClientMaxBodySize(server_config->getClientMaxBodySize());
	_validator.setPath(directive->getContentPath());
	if (!_validator.isDirectory())
		directive->setAutoIndex(0);
}

bool ConfigParser::handleLocationTokens(std::vector<std::string> tokens, std::string working_line, LocationBlock *l_directive, 
		s_parser_flags &flag, size_t line_num, ServerBlock *curr_serv, ServerConfig *serv_conf, std::ifstream &config_file, TokenCounter &token_counter) {
	if (tokens.empty())
		return (false);
	if (is_token_valid(tokens[0], B_LOC)) {
		if (tokens.size() != 2)
			throw ConfigParserError(NO_URI_LOCATION, __FUNCTION__, __LINE__, line_num);
		if (l_directive->getRoot().empty()) {
			if (curr_serv->getRoot().empty())
				throw ConfigParserError(NO_ROOT_DEFINITION, __FUNCTION__, __LINE__, line_num);
			l_directive->setRoot(curr_serv->getRoot());
		}
		processLocationBlock(config_file, working_line, token_counter, line_num, curr_serv, serv_conf);
		flag.went_in_directive = true;
		return (true);
	}
	return (false);
}

bool ConfigParser::handleDirectiveTokens(std::vector<std::string> tokens, std::string working_line, LocationBlock *location_directive,
		TokenCounter &token_counter, s_parser_flags &flag, size_t current_line) {
	if (tokens.empty())
		return (false);
	if (is_token_valid_multiple(tokens[0], _l_params) || is_token_valid_multiple(tokens[0], _c_params)) {
		if ((tokens[0] == "return" && !token_counter.isEmpty()) 
		^ (tokens[0] != "return" && token_counter.getTokenCount("return") >= 1)) {
			throw ConfigParserError(RETURN_ALLOWS_NO_DIRECTIVE, __FUNCTION__, __LINE__, current_line);
		} else if (tokens[0] == "return") {
			location_directive->setAllowedMethods(GET);
		}
		token_counter.incrementToken(tokens[0]);
		if (!token_counter.oneOccurenceCheck(_non_repeat_tokens_l))
			throw ConfigParserError(TOKEN_REPEATED, __FUNCTION__, __LINE__, current_line);
		else if (flag.went_in_directive && tokens[0] == "root")
			throw ConfigParserError(ROOT_PRIORITY, __FUNCTION__, __LINE__, current_line);
		processDirectiveLoc(location_directive, working_line, tokens, current_line);
		if (tokens[0] == "client_max_body_size")
			location_directive->clientMaxBodySizeModified();
		else if (tokens[0] == "auto_index")
			location_directive->autoIndexModified();
		return (true);
	}
	return (false);
}

void ConfigParser::processLocationBlock(std::ifstream &config_file, std::string working_line, TokenCounter &token_counter, 
		size_t &current_line, ServerBlock *current_server, ServerConfig *server_config) {
	LocationBlock *l_directive = new LocationBlock();
	std::vector<std::string> splitted_line;
	s_parser_flags flag = {false};
	std::string uri;
	size_t uri_line;
	std::streampos last_position;

	token_counter.enterBlock();
	server_config->setDirective(l_directive);
	uri_line = current_line;
	uri = returnSecondArgs(working_line);
	while (std::getline(config_file, working_line)) {
		current_line++;
		last_position = config_file.tellg();
		if (is_only_whitespace(working_line))
			continue;
		working_line = trim(working_line);
		splitted_line = split(working_line, ' ');
		if (handleLocationTokens(splitted_line, working_line, l_directive, flag, current_line, current_server, server_config, config_file, token_counter))
			continue;
		else if (is_token_valid(splitted_line[0], LOC_TERMINATOR))
			break;
		else if (handleDirectiveTokens(splitted_line, working_line, l_directive, token_counter, flag, current_line))
			continue;
		throw ConfigParserError(INVALID_TOKEN, __FUNCTION__, __LINE__, current_line);
	}
	finalizeLocationBlock(l_directive, current_server, uri, uri_line);
	config_file.seekg(last_position);
	token_counter.exitBlock();
}

void ConfigParser::processServerBlock(std::ifstream &config_file, std::string working_line, size_t &current_line, ServerConfig *server_config, size_t server_id) {
	std::vector<std::string> splitted_line;
	std::streampos last_position;
	TokenCounter token_counter;
	ServerBlock *s_directive = new ServerBlock();

	token_counter.enterBlock();
	server_config->setServerHeader(s_directive);
	while (std::getline(config_file, working_line)) {
		current_line++;
		last_position = config_file.tellg();
		if (is_only_whitespace(working_line))
			continue;
		working_line = trim(working_line);
		splitted_line = split(working_line, ' ');
		if (is_token_valid(splitted_line[0], B_LOC)) {
			if (splitted_line.size() != 2)
				throw ConfigParserError(NO_URI_LOCATION, __FUNCTION__, __LINE__, current_line);
			processLocationBlock(config_file, working_line, token_counter, current_line, s_directive, server_config);
		} else if (is_token_valid(splitted_line[0], SERVER_TERMINATOR) && splitted_line.size()) {
			break;
		} else if (is_token_valid_multiple(splitted_line[0], _c_params) || is_token_valid_multiple(splitted_line[0], _s_params)) {
			token_counter.incrementToken(splitted_line[0]);
			if (!token_counter.oneOccurenceCheck(_non_repeat_tokens_s))
				throw ConfigParserError(TOKEN_REPEATED, __FUNCTION__, __LINE__, current_line);
			processDirectiveServ(s_directive, working_line, splitted_line, current_line);
		} else
			throw ConfigParserError(INVALID_TOKEN, __FUNCTION__, __LINE__, current_line);
	}
	finalizeServerBlock(s_directive, server_config, server_id);
	config_file.seekg(last_position);
	token_counter.exitBlock();
}

void ConfigParser::parseConfigurationFile(std::ifstream &config_file) {
	std::string working_line;
	std::vector<std::string> working_line_splitted;
	size_t current_line = 0;
	size_t server_id = 0;

	while (std::getline(config_file, working_line)) {
		ServerConfig *server_config;
		Logger current_logger;
		current_line++;
		if (is_only_whitespace(working_line))
			continue;
		working_line_splitted = split(working_line, ' ');
		if (working_line_splitted[0] == B_SERVER && working_line.size()) {
			server_config = new ServerConfig();
			processServerBlock(config_file, working_line, current_line, server_config, server_id);
		} else {
			throw ConfigParserError(TOKEN_POSITION_MISMATCH, __FUNCTION__, __LINE__, current_line);
		}
		setServerConfig(server_id, server_config);
		server_id++;
	}
}

bool ConfigParser::distinctUri(ServerConfig *current_server) {
	std::vector<LocationBlock *> all_directives = current_server->getDirectives();
	std::string compared;
	std::string reference;

	for (std::size_t i = 0; i < all_directives.size(); i++) {
		for (std::size_t j = i + 1; j < all_directives.size(); j++) {
			compared =  removeTrailingSlashes(removeExcessiveSlashes(all_directives[i]->getUri()));
			reference =  removeTrailingSlashes(removeExcessiveSlashes(all_directives[j]->getUri()));
			if (compared == reference)
				return (false);
		}
	}
	return (true);
}
