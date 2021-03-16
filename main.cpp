#include <signal.h>
#include <iostream>

#include "core/Server.hpp"
#include "core/WebServ.hpp"

#include "utils/cpp_libft/libft.hpp"
#include "base64_coding/Base64.hpp"

#define SERVER_SUCCESS_STOP_LOG "\n\n¯\\_(ツ)_/¯ WebServ is stopped ¯\\_(ツ)_/¯\n\n"
#define CONFIG_FILE_DEFAULT_PATH "./WEBSERV.CONF"
#define PREFIX_DEFAULT_PATH "/default_folder/" // From this path working root


bool g_sigpipe = false;

class SuperVisor {
public:
    SuperVisor() : _webserv(NULL) { };

    void setWebServ(WebServ* webserv) { _webserv = webserv; }
    void stopServer(void) { _webserv->stop(); }

private:
    WebServ* _webserv;
};

SuperVisor supervisor;

void StopSignalHandler(int signal) {
	(void)signal;
	supervisor.stopServer();
	std::cout << SERVER_SUCCESS_STOP_LOG << std::endl;
	exit(EXIT_SUCCESS);
}

void sigPipeHandler(int signal) {
	(void)signal;
	g_sigpipe = true;
	std::cerr << "sigpipe is received" << std::endl;
}

void checkAndSetTimeZoneCorrection(void) {
    struct timeval	current;
    struct timezone tz;

    if (gettimeofday(&current, &tz)) {
        utils::exitWithLog();
    }

    WebServ::setCorrectionMinutesToGMT(tz.tz_minuteswest);
}

std::list<ServerContext*> WebServ::servers_list;
int WebServ::correction_minutes_to_GMT;
std::string WebServ::_webserv_root_path;
std::list<std::string> WebServ::_lang_code_list;
std::map<std::string, std::list<int> > WebServ::already_listening_host_plus_port;

int main(int argc, char *argv[])
{
    std::string path_to_config;
    if (argc > 2) {
        std::cout << "Please provide only 1 arg -> path to webserv config" << std::endl\
            << "or provide nothing and will be used CONFIG_FILE_DEFAULT_PATH" << std::endl;
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, StopSignalHandler);
    signal(SIGTERM, StopSignalHandler);
    signal(SIGPIPE, sigPipeHandler);

    checkAndSetTimeZoneCorrection();
    WebServ::initLanguageCodesList();

    char *absolute_path = getcwd(NULL, 0);
    if (!absolute_path) {
        utils::exitWithLog();
    }
    std::string abs_path = std::string(absolute_path);
    free(absolute_path);

    WebServ::setWebServRootPath(std::string(abs_path) + PREFIX_DEFAULT_PATH);

    if  (argc == 2) {
        path_to_config = argv[1];
    } else  {
        path_to_config = CONFIG_FILE_DEFAULT_PATH;
    }

    try
    {
        Config _config = Config(path_to_config);
        std::cout << std::endl << "===================================================" << std::endl << std::endl;

        WebServ::servers_list = _config.getServersList();
        WebServ webserv = WebServ();
        supervisor.setWebServ(&webserv);

        webserv.serveConnections();
    }
    catch (Config::BadConfigException & e)
    {
        exit(EXIT_FAILURE);
    }

    std::cout << SERVER_SUCCESS_STOP_LOG << std::endl;
    return 0;
}
