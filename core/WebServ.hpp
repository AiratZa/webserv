//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP


class WebServ;
#include <vector>
#include "Server.hpp"
#include "../config_parsing/Config.hpp"

#define  RETRY_AFTER_SECOND_DELAY "1"  // for header Retry-After

#define DEFAULT_RESPONSE_CHARSET "utf-8"
#define DEFAULT_RESPONSE_LANGUAGE "en-US"

#define CHECK_ACCEPT_CHARSET_HEADER 1
#define CHECK_ACCEPT_LANGUAGE_HEADER 1

class WebServ {
private:
    std::vector<Server*> _servers;

    static int correction_minutes_to_GMT; // TIMEZONE
    static std::string _webserv_root_path;
    static std::map<std::string, std::list<int> > already_listening_host_plus_port;
    static std::list<std::string> _lang_code_list;
    int _max_fd;

public:
    WebServ();
    ~WebServ(void);

    static std::list<ServerContext*> servers_list;

    int                                     getServersCount(void) const { return _servers.size(); }
    Server*                                 getServerByPosition(int i) { return _servers[i];}
    static std::list<ServerContext*>        getAllAsteriskHostPortComboList(const int port);
    static int                              getCorrectionMinutesToGMT(void) { return correction_minutes_to_GMT;}
    static const std::string&               getWebServRootPath(void) { return _webserv_root_path;}
    static std::list<std::string>&          getLanguageCodesList(void) { return _lang_code_list;}
    static std::list<ServerContext*>        getAllExactHostPortComboList(const std::string& host, const int port);
    const int &                             getMaxFD() const { return _max_fd; }

    static void                             setCorrectionMinutesToGMT(int min) { correction_minutes_to_GMT = min;}
    static void                             setWebServRootPath(const std::string& path) { _webserv_root_path = path;}
    static void                             setInAlreadyListeningHostPlusPort(const std::string& host_str, int port) {
                                                    already_listening_host_plus_port[host_str].push_back(port);}

    void    updateMaxFD(void);
    void    addServer(Server* server);
	void    serveConnections();
	void    stop();

    static ServerContext*   findServerForHandlingRequest(const std::string& host,
                                                       const int port,
                                                       const std::string& server_name);

    static void             routeRequest(const std::string& host, const int port,
                                                         Request* _client_request,
                                                         const std::string& request_target);

    static bool             isAlreadyListeningHostPlusPort(const std::string& host_str, int port);

    class NotOKStatusCodeException: public std::exception {
        virtual const char* what() const throw() {return "";}
    };

	static LocationContext* searchForBestMatchLocation(ServerContext* handling_server,
														 Request* current_request,
														 const std::string& request_target);
	static bool isPartOfLocationPath(const std::list<std::string>& request_target, const std::list<std::string>& location_path);
	static int isServerNameMatch(const std::string& server_name, const std::list<std::string>& serv_name_list);

    static void initLanguageCodesList(void)
    {
        _lang_code_list.push_back("af");
        _lang_code_list.push_back("ak");
        _lang_code_list.push_back("sq");
        _lang_code_list.push_back("am");
        _lang_code_list.push_back("ab");
        _lang_code_list.push_back("ar");
        _lang_code_list.push_back("aa");
        _lang_code_list.push_back("an");
        _lang_code_list.push_back("hy");
        _lang_code_list.push_back("as");
        _lang_code_list.push_back("av");
        _lang_code_list.push_back("ae");
        _lang_code_list.push_back("ay");
        _lang_code_list.push_back("az");
        _lang_code_list.push_back("bm");
        _lang_code_list.push_back("ba");
        _lang_code_list.push_back("eu");
        _lang_code_list.push_back("be");
        _lang_code_list.push_back("bn");
        _lang_code_list.push_back("bh");
        _lang_code_list.push_back("bi");
        _lang_code_list.push_back("bs");
        _lang_code_list.push_back("br");
        _lang_code_list.push_back("bg");
        _lang_code_list.push_back("my");
        _lang_code_list.push_back("ca");
        _lang_code_list.push_back("ch");
        _lang_code_list.push_back("ce");
        _lang_code_list.push_back("ny");
        _lang_code_list.push_back("zh");
        _lang_code_list.push_back("cv");
        _lang_code_list.push_back("kw");
        _lang_code_list.push_back("co");
        _lang_code_list.push_back("cr");
        _lang_code_list.push_back("hr");
        _lang_code_list.push_back("cs");
        _lang_code_list.push_back("da");
        _lang_code_list.push_back("dv");
        _lang_code_list.push_back("nl");
        _lang_code_list.push_back("dz");
        _lang_code_list.push_back("en");
        _lang_code_list.push_back("eo");
        _lang_code_list.push_back("et");
        _lang_code_list.push_back("ee");
        _lang_code_list.push_back("fo");
        _lang_code_list.push_back("fj");
        _lang_code_list.push_back("fi");
        _lang_code_list.push_back("fr");
        _lang_code_list.push_back("ff");
        _lang_code_list.push_back("gl");
        _lang_code_list.push_back("ka");
        _lang_code_list.push_back("de");
        _lang_code_list.push_back("el");
        _lang_code_list.push_back("gn");
        _lang_code_list.push_back("gu");
        _lang_code_list.push_back("ht");
        _lang_code_list.push_back("ha");
        _lang_code_list.push_back("he");
        _lang_code_list.push_back("hz");
        _lang_code_list.push_back("hi");
        _lang_code_list.push_back("ho");
        _lang_code_list.push_back("hu");
        _lang_code_list.push_back("ia");
        _lang_code_list.push_back("id");
        _lang_code_list.push_back("ie");
        _lang_code_list.push_back("ga");
        _lang_code_list.push_back("ig");
        _lang_code_list.push_back("ik");
        _lang_code_list.push_back("io");
        _lang_code_list.push_back("is");
        _lang_code_list.push_back("it");
        _lang_code_list.push_back("iu");
        _lang_code_list.push_back("ja");
        _lang_code_list.push_back("jv");
        _lang_code_list.push_back("kl");
        _lang_code_list.push_back("kn");
        _lang_code_list.push_back("kr");
        _lang_code_list.push_back("ks");
        _lang_code_list.push_back("kk");
        _lang_code_list.push_back("km");
        _lang_code_list.push_back("ki");
        _lang_code_list.push_back("rw");
        _lang_code_list.push_back("ky");
        _lang_code_list.push_back("kv");
        _lang_code_list.push_back("kg");
        _lang_code_list.push_back("ko");
        _lang_code_list.push_back("ku");
        _lang_code_list.push_back("kj");
        _lang_code_list.push_back("la");
        _lang_code_list.push_back("lb");
        _lang_code_list.push_back("lg");
        _lang_code_list.push_back("li");
        _lang_code_list.push_back("ln");
        _lang_code_list.push_back("lo");
        _lang_code_list.push_back("lt");
        _lang_code_list.push_back("lu");
        _lang_code_list.push_back("lv");
        _lang_code_list.push_back("gv");
        _lang_code_list.push_back("mk");
        _lang_code_list.push_back("mg");
        _lang_code_list.push_back("ms");
        _lang_code_list.push_back("ml");
        _lang_code_list.push_back("mt");
        _lang_code_list.push_back("mi");
        _lang_code_list.push_back("mr");
        _lang_code_list.push_back("mh");
        _lang_code_list.push_back("mn");
        _lang_code_list.push_back("na");
        _lang_code_list.push_back("nv");
        _lang_code_list.push_back("nd");
        _lang_code_list.push_back("ne");
        _lang_code_list.push_back("ng");
        _lang_code_list.push_back("nb");
        _lang_code_list.push_back("nn");
        _lang_code_list.push_back("no");
        _lang_code_list.push_back("ii");
        _lang_code_list.push_back("nr");
        _lang_code_list.push_back("oc");
        _lang_code_list.push_back("oj");
        _lang_code_list.push_back("cu");
        _lang_code_list.push_back("om");
        _lang_code_list.push_back("or");
        _lang_code_list.push_back("os");
        _lang_code_list.push_back("pa");
        _lang_code_list.push_back("pi");
        _lang_code_list.push_back("fa");
        _lang_code_list.push_back("pl");
        _lang_code_list.push_back("ps");
        _lang_code_list.push_back("pt");
        _lang_code_list.push_back("qu");
        _lang_code_list.push_back("rm");
        _lang_code_list.push_back("rn");
        _lang_code_list.push_back("ro");
        _lang_code_list.push_back("ru");
        _lang_code_list.push_back("sa");
        _lang_code_list.push_back("sc");
        _lang_code_list.push_back("sd");
        _lang_code_list.push_back("se");
        _lang_code_list.push_back("sm");
        _lang_code_list.push_back("sg");
        _lang_code_list.push_back("sr");
        _lang_code_list.push_back("gd");
        _lang_code_list.push_back("sn");
        _lang_code_list.push_back("si");
        _lang_code_list.push_back("sk");
        _lang_code_list.push_back("sl");
        _lang_code_list.push_back("so");
        _lang_code_list.push_back("st");
        _lang_code_list.push_back("es");
        _lang_code_list.push_back("su");
        _lang_code_list.push_back("sw");
        _lang_code_list.push_back("ss");
        _lang_code_list.push_back("sv");
        _lang_code_list.push_back("ta");
        _lang_code_list.push_back("te");
        _lang_code_list.push_back("tg");
        _lang_code_list.push_back("th");
        _lang_code_list.push_back("ti");
        _lang_code_list.push_back("bo");
        _lang_code_list.push_back("tk");
        _lang_code_list.push_back("tl");
        _lang_code_list.push_back("tn");
        _lang_code_list.push_back("to");
        _lang_code_list.push_back("tr");
        _lang_code_list.push_back("ts");
        _lang_code_list.push_back("tt");
        _lang_code_list.push_back("tw");
        _lang_code_list.push_back("ty");
        _lang_code_list.push_back("ug");
        _lang_code_list.push_back("uk");
        _lang_code_list.push_back("ur");
        _lang_code_list.push_back("uz");
        _lang_code_list.push_back("ve");
        _lang_code_list.push_back("vi");
        _lang_code_list.push_back("vo");
        _lang_code_list.push_back("wa");
        _lang_code_list.push_back("cy");
        _lang_code_list.push_back("wo");
        _lang_code_list.push_back("fy");
        _lang_code_list.push_back("xh");
        _lang_code_list.push_back("yi");
        _lang_code_list.push_back("yo");
        _lang_code_list.push_back("za");
        _lang_code_list.push_back("zu");
    }
};

#endif //WEBSERV_WEBSERV_HPP
