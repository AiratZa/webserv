#include "Server.hpp"
#include "WebServ.hpp"
#include <signal.h>
#include "utils/cpp_libft/libft.hpp"
#include <iostream>

#include "response/autoindex_handling/autoindex_handling.hpp" // TODO: will be removed after TESTS

#include "base64_coding/base64.hpp"

#define SERVER_SUCCESS_STOP_LOG "\n\n¯\\_(ツ)_/¯ WebServ is stopped ¯\\_(ツ)_/¯\n\n"
#define CONFIG_FILE_DEFAULT_PATH "./WEBSERV.CONF"
#define PREFIX_DEFAULT_PATH "/default_folder/" // From this path working root // TODO:add ending '/'


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
        WebServ webserv = WebServ(&_config);
        supervisor.setWebServ(&webserv);

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

    std::cout << SERVER_SUCCESS_STOP_LOG << std::endl;
    return 0;
}

////TODO: make connections persistent
//TODO: accept all incoming connettions in Listener::acceptConnection(), not only one

////TODO: set timeout in select
////TODO: close connection after sending 400, frc 7230 3.3.3-4
////TODO: make autoindex
////TODO: when there is cgi_script there must be also cgi_ext
////TODO: moving back with ../ in autoindex

////todo: not found or forbidden (test returns 404 when there is index set in location in config, but it is not found; but nginx returns 403 )
////todo: config file: index "345 6"; => unexpected end of file, expecting ";" or "}"
////todo: location /ht matches http://localhost:8080/htlsakflsda
////todo: Test GET http://localhost:8080/directory/youpi.bla doesnt work with cgi_tester
////todo: dont know how error_page work
////todo: unlink temp files in cgi
////todo: "/Users/jnannie/Desktop/webserv/default_folder/html/directory/youpi.bla" in cgi, directory should be replaced by alias

////todo: check cgi script when extension is set in config
////todo: what if autoindex is folder http://localhost:8085/qq/tt http://localhost:8080/234345/
////TODO: check accept-charset, accept-language
// TODO: and other request headers before response
////todo: searchForErrorPageLinkAndSetChangeError() gives error
////todo: Test multiple workers(20) doing multiple times(5): Put on /put_test/multiple_same with size 1000000; the file multiple_same should exists with a size of 1000000 bytes // we have 20Mb file
////todo: send response not in one chunk
////todo: when pressing to link "../" in autoindex page "http://localhost:8080/234/345/" url is becoming "http://localhost:8080/234//345/"; or when url is "http://localhost:8080/234/"
////todo: check for sigpipe in sendResponse
////todo: close connection if recv returns 0
////todo: make authentication not only for "/ht" in Listener::processHeaderInfoForActions()
//todo: test with several servers
////todo: make content-length parsing in cgi
//todo: make copliens form
//todo: add languageS to content-language and remove _lang_
//todo: content-location
//todo: check Response::generatePostResponse()
//todo: put request fail
//todo: close sockets with ctrl+c + listener socket



/* todo:
< location: http://localhost:8081/post_body/nginx_meme.jpg/
< Content-Language: en-US
<
* Connection #0 to host localhost left intact
va-e5% curl -T nginx_meme.jpg http://localhost:8081/post_body/ -v
 */


//location /ht {
//auth_basic "closed site";
//auth_basic_uset_file passwd;
//}

/*TODO:
 *
 * va-e5% curl -T nginx_meme.jpg http://localhost:8081/post_body -v
*   Trying 127.0.0.1...
* TCP_NODELAY set
* Connected to localhost (127.0.0.1) port 8081 (#0)
> PUT /post_body HTTP/1.1
> Host: localhost:8081
> User-Agent: curl/7.54.0
> Accept: *
> Content-Length: 172172
> Expect: 100-continue
>
< HTTP/1.1 409 CONFLICT
		< server: webserv
		< date: Sun, 14 Mar 2021 15:48:33 GMT
		< Content-Type: text/html;charset=utf-8
										  < Content-Length: 14
< Content-Language: en-US
		* HTTP error before end of send, stop sending
		<
409 CONFLICT

*/

// Can't assign requested address TODO:

/* TODO:
 * https://nginx.org/ru/docs/http/request_processing.html
 * http://nginx.org/ru/docs/example.html
 * server {
    listen      80;
    server_name example.org www.example.org;
    ...
}

server {
    listen      80;
    server_name example.net www.example.net;
    ...
}

server {
    listen      80;
    server_name example.com www.example.com;
    ...
}
 */