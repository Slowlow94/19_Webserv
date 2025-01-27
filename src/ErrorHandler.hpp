#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP

# include <string>
# include <exception>
# include <sstream>

class Error : public std::exception
{
	protected:
		std::string _msg;
	public:
		explicit Error(const std::string& msg);
		explicit Error(void);
		virtual const char* what() const throw();
		virtual ~Error() throw();
};

class ServerHandlerError : public Error
{
	public:
		explicit ServerHandlerError(const std::string& msg, const char* function, int line);
		virtual ~ServerHandlerError() throw();
};

class ServerBaseError : public Error
{
	public:
		explicit ServerBaseError(const std::string& msg, const char* function, int line);
		virtual ~ServerBaseError() throw();
};

class ConfigParserError : public Error
{
	public:
   		explicit ConfigParserError(const std::string& msg, const char* function, int line, int config_line = -1);
   		virtual ~ConfigParserError() throw();
};

std::string toStrInt(int value);
std::string toStr(const char* value);


#endif
