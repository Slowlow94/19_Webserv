#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <ctime>
# include <fstream>
# include <string>
# include <iostream>

// COLORS MACRO
#ifndef RESET
	#define RESET	"\x1b[0m"
#endif
#define RED		"\x1b[31m"
#define GREEN	"\x1b[32m"
#define YELLOW	"\x1b[33m"
#define CYAN	"\x1b[36m"

// LOG LEVEL MACROS
#define INFO		"[INFO]"
#define WARN		"[WARN]"
#define ERROR		"[ERROR]"
#define CRITICAL 	"[CRIT]" 

class Logger
{
	private:
		std::string		outputTimestamp(void) ;
		void			genericMessage(std::string color, std::string header, std::string msg, std::ostream &output_stream);
	public:
		Logger(void);
		~Logger(void);
		Logger(const Logger& copy);
		Logger& operator=(const Logger& assign);
		void info(std::string msg);
		void warn(std::string msg);
		void error(std::string msg);
		void critical(std::string msg);
};

#endif
