#include "Server.hpp"
#include "WebServ.hpp"
#include <signal.h>
#include "utils/cpp_libft/libft.hpp"
#include <iostream>

#include "response/autoindex_handling/autoindex_handling.hpp" // TODO: will be removed after TESTS

#include "base64_coding/base64.hpp"


#define CONFIG_FILE_DEFAULT_PATH "./WEBSERV.CONF"
#define PREFIX_DEFAULT_PATH "/default_folder/" // From this path working root // TODO:add ending '/'

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



std::list<ServerContext*> WebServ::servers_list;
int WebServ::correction_minutes_to_GMT;
std::string WebServ::_webserv_root_path;

int main(int argc, char *argv[])
{
    std::string path_to_config;
    if (argc > 2) {
        std::cout << "Please provide only 1 arg -> path to webserv config" << std::endl\
            << "or provide nothing and will be used CONFIG_FILE_DEFAULT_PATH" << std::endl;
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, intHandler);
    checkAndSetTimeZoneCorrection();


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
//    write_html(); // TODO: Autoindex temp testing

    try
    {
        Config _config = Config(path_to_config);
        std::cout << std::endl << "===================================================" << std::endl << std::endl;

        WebServ::servers_list = _config.getServersList();
        webserv = WebServ(_config);

//        std::string test("");
//		std::cout << "encoded: " << Base64::base64_encode(test) << std::endl;
//		std::string encoded = Base64::base64_encode(test);
//		std::cout << "decoded: " << Base64::base64_decode(encoded) << std::endl;

        webserv.serveConnections();
    }
    catch (Config::BadConfigException & e)
    {
        exit(EXIT_FAILURE);
    }


    return 0;
}

//TODO: make connections persistent
//TODO: accept all incoming connettions in Listener::acceptConnection(), not only one
//TODO: check accept-charset, accept-language and other request headers before response
//TODO: set timeout in select
//TODO: close connection after sending 400, frc 7230 3.3.3-4
//TODO: check "Если разрешён метод GET, то метод HEAD также будет разрешён."
//TODO: dont forget about cgi bug

//location /ht {
//auth_basic "closed site";
//auth_basic_uset_file passwd;
//}

