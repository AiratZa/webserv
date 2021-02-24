#include "Server.hpp"
#include "WebServ.hpp"
#include <signal.h>
#include "utils/cpp_libft/libft.hpp"
#include <iostream>

#include "response/autoindex_handling/autoindex_handling.hpp" // TODO: will be removed after TESTS


#define CONFIG_FILE_DEFAULT_PATH "./WEBSERV.CONF"
#define PREFIX_DEFAULT_PATH "/default_folder" // From this path working root

WebServ webserv;

void intHandler(int signal) {
	(void)signal;
	webserv.stop();
	exit(EXIT_SUCCESS);
}

void checkAndSetTimeZoneCorrection(void) {
    struct timeval	current;
    struct timezone tz;

    if (gettimeofday(&current, &tz)) {
        utils::exitWithLog();
    }

    WebServ::setCorrectionMinutesToGMT(tz.tz_minuteswest);
}

// set global root path
void setGlobalRootPath(void) {
    char *absolute_path = getcwd(NULL, 0);
    if (!absolute_path) {
        utils::exitWithLog();
    }

    WebServ::setWebServRootPath(std::string(absolute_path) + PREFIX_DEFAULT_PATH);
}

std::list<ServerContext*> WebServ::servers_list;
int WebServ::correction_minutes_to_GMT;
std::string WebServ::_webserv_root_path;

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

    checkAndSetTimeZoneCorrection();
    setGlobalRootPath();

//    write_html(); // TODO: Autoindex temp testing

    try
    {
        Config _config = Config(path_to_config);
        std::cout << std::endl << "===================================================" << std::endl << std::endl;

        WebServ::servers_list = _config.getServersList();
        webserv = WebServ(_config);
        webserv.serveConnections();
    }
    catch (Config::BadConfigException & e)
    {
        exit(EXIT_FAILURE);
    }


    return 0;
}
