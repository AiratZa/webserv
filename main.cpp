#include "Server.hpp"
#include "WebServ.hpp"
#include <signal.h>

WebServ webserv;

void intHandler(int signal) {
	(void)signal;
	webserv.stop();
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    std::string path_to_config;
    if  (argc == 2) {
        path_to_config = argv[1];
    } else if (argc == 1) {
        path_to_config = CONFIG_FILE_DEFAULT_PATH;
    } else {
        std::cout << "Please provide only 1 arg -> path to webserv config" << std::endl\
            << "or provide nothing and will be used CONFIG_FILE_DEFAULT_PATH" << std::endl;
        exit(EXIT_FAILURE);
    }
	signal(SIGINT, intHandler);
	webserv = WebServ(path_to_config);
	webserv.serveConnections();
    return 0;
}
