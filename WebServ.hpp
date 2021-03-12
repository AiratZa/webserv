//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include <vector>

class WebServ;
#include "Server.hpp"
#include "config_parsing/Config.hpp"

#define DEFAULT_RESPONSE_CHARSET "utf-8"
#define DEFAULT_RESPONSE_LANGUAGE "en-US"

#define CHECK_ACCEPT_CHARSET_HEADER 1
#define CHECK_ACCEPT_LANGUAGE_HEADER 1


class WebServ {
public:
    WebServ() { }
    WebServ(const Config& config);
    void addServer(Server* server);
    int getServersCount(void) const { return _servers.size(); }
    Server* getServerByPosition(int i);

//    fd_set* getReadSetPtr(void) { return &_readset; };
//    fd_set* getWriteSetPtr(void) { return &_writeset; };

    void setToReadFDSet(const std::list<int>& clientsFD);
    void setToWriteFDSet(const std::list<int>& clientsFD);

    void updateMaxFD(void);
    const int & getMaxFD() const { return _max_fd; }

	void serveConnections();

    static ServerContext* findServerForHandlingRequest(const std::string& host,
                                                         const int port,
                                                         const std::string& server_name);

    static void routeRequest(const std::string& host, const int port, Request* _client_request);
//    static void routeRequests(const std::string& host, const int port, std::map<int, Request *>& _clients_requests); // jnannie: not needed

	void stop();

    static std::list<ServerContext*> servers_list;

    static int getCorrectionMinutesToGMT(void) { return correction_minutes_to_GMT;}
    static void setCorrectionMinutesToGMT(int min) { correction_minutes_to_GMT = min;}

    static const std::string& getWebServRootPath(void) {
        return _webserv_root_path;
    }

    static void setWebServRootPath(const std::string& path) {
        _webserv_root_path = path;
    }

    static std::list<std::string>& getCharsetList(void) { return _charset_list; }
    static void initCharsetList(void);


private:
    std::vector<Server*> _servers;

    static int correction_minutes_to_GMT; // TIMEZONE
    static std::string _webserv_root_path;

    static std::list<std::string> _charset_list; // https://www.iana.org/assignments/character-sets/character-sets.txt

    Config _config;
    int _max_fd;

};


#endif //WEBSERV_WEBSERV_HPP
