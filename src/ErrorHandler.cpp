#include "ErrorHandler.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

std::string toStr(const char* value)
{
    std::ostringstream	oss;

    oss << value;
    return oss.str();
}

std::string toStrInt(int value)
{
    std::ostringstream	oss;

    oss << value;
    return oss.str();
}

Error::Error(const std::string& msg) : _msg(msg) {};
const char* Error::what() const throw() { return _msg.c_str(); }
Error::~Error() throw() {}

ServerHandlerError::ServerHandlerError(const std::string& msg, const char* function, int line) : Error("Error in ServerHandler class in function : " + toStr(function) + " in line : " + toStrInt(line) + " : " + msg) {};
ServerHandlerError::~ServerHandlerError() throw() {}

ServerBaseError::ServerBaseError(const std::string& msg, const char* function, int line) : Error("Error in ServerBase class in function : " + toStr(function) + " in line : " + toStrInt(line) + " : " + msg) {};
ServerBaseError::~ServerBaseError() throw() {}

ConfigParserError::ConfigParserError(const std::string& msg, const char* function, int line, int config_line)
    : Error(msg + '\n'
            + "---" + '\n' +
            "Config file line error : " + toStrInt(config_line) + '\n' +
            "Exception from ConfigParser method : " + toStr(function) + '\n' +
            "at line : " + toStrInt(line)) {}

ConfigParserError::~ConfigParserError() throw() {}
