#include "Logger.hpp"

Logger::Logger(void) {}

Logger::~Logger(void) {}

std::string Logger::outputTimestamp(void)
{
	char 		timestamp[20];
	std::time_t	now = std::time(NULL);
	std::tm*	now_tm = std::localtime(&now);
	std::strftime(timestamp, sizeof(timestamp), "%Y/%m/%d - %H:%M", now_tm);
	std::string result(timestamp);
	return (result);
}

void Logger::genericMessage(std::string color, std::string header, std::string msg, std::ostream &output_stream) {
	std::string fully_fledged_message;
	std::string log_message;
	std::string timestamp = outputTimestamp();
	fully_fledged_message = color + header + " [" + timestamp + "] "  + ": " + msg + RESET + '\n';
	output_stream << fully_fledged_message;
	output_stream.flush();
}

void Logger::info(std::string msg) {
	genericMessage(GREEN, INFO, msg, std::cout);
}

void Logger::warn(std::string msg) {
	genericMessage(CYAN, WARN, msg, std::cerr);
}

void Logger::error(std::string msg) {
	genericMessage(YELLOW, ERROR,msg, std::cerr);

}

void Logger::critical(std::string msg) {
	genericMessage(RED, CRITICAL, msg, std::cerr);
}
