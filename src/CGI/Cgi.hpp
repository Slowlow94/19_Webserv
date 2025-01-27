
#ifndef CGI_HPP
# define CGI_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <map>
#include <fstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

class RRState;

class Cgi
{
    private:
    public:
        Cgi();
        ~Cgi();
        std::string         getQuery(std::string path);
        void                handleCGI(RRState& rrstate, std::string route ,std::string path);
        std::string         getClientIP(RRState& rrstate);
        int                 getClientPort(RRState& rrstate);
        void                handleCgiResponse(std::string output, RRState& rrstate);
        std::string         findAccept(std::map<std::string, std::string> headers);
        std::string         readDatasFromScript(int pipefd);
        std::vector<char *> homeMadeSetEnv(RRState& rrstate, std::string scriptPath, std::string path);
};

#endif
