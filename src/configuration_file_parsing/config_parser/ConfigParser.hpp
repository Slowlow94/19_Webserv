#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <limits.h>
#include <stdlib.h>
#include <cctype>
#include "../server_config/ServerConfig.hpp"
#include "../utils/TokenCounter.hpp"

#define SERVER_TERMINATOR	"end-server"
#define LOC_TERMINATOR		"end-location"
#define B_SERVER			"server"
#define B_LOC				"location"
#define FILE_EXT			".conf"

// TOKENS
#define S_BLOCK_TOKENS "server_name listen error_pages"
#define L_BLOCK_TOKENS "cgi_path alias allowed_method depends_on return"
#define C_TOKENS "root index auto_index client_max_body_size"
#define L_NO_RPT_TOKENS "root cgi_path alias depends_on"
#define S_NO_RPT_TOKENS "root"

#ifndef IS_LINUX
	#define IS_LINUX 0
#endif

#if defined(__linux__)
    #undef IS_LINUX
    #define IS_LINUX 1
#endif

typedef struct s_parser_flags {
	bool went_in_directive;
}	t_parser_flags;

class ConfigParser {
	private:
		std::vector <std::string>			_l_params;
		std::vector <std::string>			_c_params;
		std::vector <std::string>			_s_params;
		std::vector <std::string>			_non_repeat_tokens_l;
		std::vector <std::string>			_non_repeat_tokens_s;
		std::set <unsigned int>				_ports;
		static ConfigParser*				_instance;
		std::map<size_t , ServerConfig *>	_servers_config;
		PathValidator						_validator;
		Logger								_logger;
		
		// Constructor
		ConfigParser(const std::string init_path);

		// Utils
		std::string					trim(const std::string& str);
		std::vector<std::string>	split(const std::string& str, char delimiter);
		bool						is_only_whitespace(const std::string& str);
		bool						is_token_valid_multiple(const std::string& line, const std::vector <std::string> tokens);
		bool						is_token_valid(const std::string& line, const std::string token);
		void						initializeVector(std::vector<std::string>& vec, std::string tokens);
		bool						endsWith(const std::string path, const std::string extension);
		bool						isStringDigit(std::string args);
		bool						isValidServerName(std::string name);
		bool						isValidURL(const std::string& url);
		std::string					returnSecondArgs(std::string args);
		std::string					removeExcessiveSlashes(const std::string& path);
		bool						distinctUri(ServerConfig *current_server);
		std::string					simplifyPath(const std::string& path);
		std::string 				toStrInt(int value);
		bool						checkDependsOn(ServerConfig *current_config);
		bool						checkAlias(ServerConfig *current_config);
		bool						CgiPathChecker(LocationBlock *l_block);
		void						validateCgiPaths(ServerConfig *serv_conf);
		std::string					removeTrailingSlashes(const std::string& input);

		// Config File parsing
		bool handleDirectiveTokens(std::vector<std::string> tokens, std::string working_line, LocationBlock *location_directive, TokenCounter &token_counter, s_parser_flags &flag, size_t current_line);
		bool handleLocationTokens(std::vector<std::string> tokens, std::string working_line, LocationBlock *location_directive, s_parser_flags &flag, size_t current_line, ServerBlock *current_server, ServerConfig *server_config, std::ifstream &config_file, TokenCounter &token_counter);
		void parseConfigurationFile(std::ifstream &configuration_file);
		void processLocationBlock(std::ifstream &config_file, std::string w_line, TokenCounter &Tk,  size_t &current_line, ServerBlock *current_server, ServerConfig *server_config);
		void finalizeLocationBlock(LocationBlock *directive, ServerBlock *server_config, std::string location_line, size_t current_line);
		void finalizeServerBlock(ServerBlock *directive, ServerConfig *serv_conf, size_t server_id);
		void processServerBlock(std::ifstream &config_file, std::string w_line,  size_t &current_line, ServerConfig *server_config, size_t server_id);

		// Directive parsing
		void processCommonDirective(ADirective *directive, std::string working_line, std::vector<std::string> args, size_t current_line);
		void processDirectiveLoc(LocationBlock *directive, std::string working_line, std::vector<std::string> args, size_t current_line);
		void processDirectiveServ(ServerBlock *directive,  std::string working_line, std::vector<std::string> args, size_t current_line);
		void parseRoot(std::string working_line, ADirective *directive, size_t current_line);
		void parseIndex(std::vector <std::string> working_line, ADirective *directive, size_t current_line);
		void parseAutoIndex(std::vector<std::string> args, ADirective *directive, size_t current_line);
		void parseClientMaxBodySize(std::vector <std::string> args, ADirective *directive, size_t current_line);
		void parseCgiPath(std::string working_line, LocationBlock *directive, size_t current_line);
		void parseAlias(std::string working_line, LocationBlock *directive, size_t current_line);
		void parseAllowedMethhod(std::vector <std::string> args, LocationBlock *directive, size_t current_line);
		void parseServerName(std::vector <std::string> args, ServerBlock *directive, size_t current_line);
		void parseListeningPorts(std::vector <std::string> args, ServerBlock *directive, size_t current_line);
		bool checkErrorPagesAvailability(std::string path);
		void parseErrorPages(std::vector <std::string> args, ServerBlock *directive, size_t current_line);
		void parseDependsOn(std::vector <std::string> args, LocationBlock *directive, size_t current_line);
		void parseReturn(std::vector <std::string> args, LocationBlock *directive, size_t current_line);

	public:
		~ConfigParser();
		static ConfigParser*				getInstance(const std::string init_path);
		ServerConfig*						getServerConfig(unsigned int id) const;
		std::map <size_t, ServerConfig *>	getAllServerConfig(void) const;
		void								setServerConfig(size_t server_id, ServerConfig *current_server);
};

#endif