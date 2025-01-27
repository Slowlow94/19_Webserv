#ifndef PATHVALIDATOR_HPP
# define PATHVALIDATOR_HPP

#include <string>
#include <sys/stat.h>
#include <unistd.h>

class PathValidator {
	private:
		std::string _path;
	public:
		PathValidator(void);
		PathValidator(const std::string &path);
		void setPath(const std::string &path);
		bool exists(void) const;
		bool isFile(void) const;
		bool isDirectory(void) const;
		bool isReadable(void) const;
		bool isWritable(void) const;
		bool isExecutable(void) const;
};

#endif