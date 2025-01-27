#ifndef CONFIGEXCEPTION_HPP
# define CONFIGEXCEPTION_HPP

#include <iostream>
#include "../../ErrorHandler.hpp"

// COMMON MACROS
#define ERROR_HEADER "\e[91m[CONFIG_PARSER | ERROR] : "
#define AB " aborting."

#ifndef RESET
    #define RESET "\e[0m"
#endif

// PATH RELATED ERROR MESSAGES
#define BAD_ACCESS ERROR_HEADER "No such file present," AB RESET
#define BAD_CONFIG_FILE ERROR_HEADER "Config file isn't readable," AB RESET
#define NO_SERVER_CONFIG ERROR_HEADER "No such server configuration," AB RESET
#define PATH_NOT_RECOGNIZED ERROR_HEADER "Path isn't recognized," AB RESET
#define PATH_ALREADY_DEFINED ERROR_HEADER "Path is already defined," AB RESET
#define ABSOULTE_ROOT ERROR_HEADER "Root path should be absolute," AB RESET
#define RELATIVE_ERROR_NO_ROOT ERROR_HEADER "Error pages lacks root in the server level to base its URI," AB RESET
#define URI_STYLE_FORMAT_ERROR ERROR_HEADER "Error pages uses either an absolute path, or an URI based on the existing root," AB RESET

// CONFIG PARSER RULES ERROR MESSAGES
#define INVALID_TOKEN ERROR_HEADER  "Invalid token in the configuration file," AB RESET
#define TOKEN_REPEATED ERROR_HEADER "Non-repeatable token repeated within block," AB RESET
#define BAD_INSTRUCTION ERROR_HEADER "Bad instruction within a directive," AB RESET
#define TOKEN_POSITION_MISMATCH ERROR_HEADER "Token is not within any block," AB RESET
#define NO_INSTRUCTION ERROR_HEADER "No instruction given in the configuration file," AB RESET
#define DUPE_ELEMS ERROR_HEADER "Duplicate elements discovered," AB RESET
#define NO_ELEMENTS ERROR_HEADER "Lack of proper arguments," AB RESET
#define DOUBLE_DIRECTIVE ERROR_HEADER "Double server directive," AB RESET
#define ROOT_PRIORITY ERROR_HEADER "Root redefinition should be declared before inbricked location," AB RESET
#define NO_ROOT_DEFINITION ERROR_HEADER "No root definition, vacant URI prefix," AB RESET
#define UNDEFINED_PARAMS ERROR_HEADER "Undefined parameter," AB RESET
#define NUMERICAL_VALUE_EXPECTED ERROR_HEADER "Numerical value expected," AB RESET
#define AMBIGUOUS_URI_DEFINITION ERROR_HEADER "Root and alias cannot be declared inside the same location block," AB RESET
#define DOUBLE_LOCATION_URI ERROR_HEADER "Repeated URI," AB RESET
#define SERVER_NAME_ONE_NAME ERROR_HEADER "The directive server_name takes one and only one server name," AB RESET
#define DEPENDS_ON_NO_MATCH ERROR_HEADER "No match found for depends on directive," AB RESET
#define ALIAS_NO_MATCH ERROR_HEADER "No match found for alias directive," AB RESET
#define BAD_CGI_PATH ERROR_HEADER "No corresponding path for the CGI," AB RESET
#define CGI_PATH_HAS_SUBROUTE ERROR_HEADER "CGI Path cannot have a subroute," AB RESET
#define WRONG_RETURN_SCOPE ERROR_HEADER "Return handles redirection. In this context it doesn't support other code from 300-308," AB RESET
#define WRONG_URL_REDIR ERROR_HEADER "Wrong URL format for redirection," AB RESET
#define RETURN_ALLOWS_NO_DIRECTIVE ERROR_HEADER "Return allows no other directive than itself in the location block," AB RESET

// SERVER HEADER ERROR MESSAGES
#define NOT_VALID_SERVER_NAME ERROR_HEADER "Invalid server name," AB RESET
#define SERVER_NAME_DUPE ERROR_HEADER "Duplicate server name," AB RESET
#define EXCEEDING_LIMIT ERROR_HEADER "Size limit exceeded," AB RESET
#define PORT_SCOPE_LINUX ERROR_HEADER "Port scope is limited to 1024 - 65.535 for linux," AB RESET
#define PORT_SCOPE_GENERAL ERROR_HEADER "Port scope is limited to 0 - 65.535 for your system," AB RESET 
#define PORT_DUPE ERROR_HEADER "Port already declared in other server configuration," AB RESET
#define PORT_NOT_SET ERROR_HEADER "Port was not set in a server config," AB RESET
#define WRONG_ERROR_PAGES_SCOPE ERROR_HEADER "Wrong error pages scope," AB RESET
#define SERVER_NAME_NOT_SET ERROR_HEADER "Server name not set," AB RESET

// LOCATION RELATED ERROR MESSAGES
#define NO_URI_LOCATION ERROR_HEADER "Location token contain no URI," AB RESET
#define BAD_URI ERROR_HEADER "Bad URI," AB RESET

#endif