//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include <vector>

#include "Server.hpp"
#include "config_parsing/Config.hpp"


class WebServ {
public:
    WebServ() { }
    WebServ(const Config& config);
    void addServer(Server* server);
    int getServersCount(void) const { return _servers.size(); }
    Server* getServerByPosition(int i);

    fd_set* getReadSetPtr(void) { return &_readset; };
    fd_set* getWriteSetPtr(void) { return &_writeset; };

    void setToReadFDSet(const std::list<int>& clientsFD);
    void setToWriteFDSet(const std::list<int>& clientsFD);

    void updateMaxFD(void);
    const int & getMaxFD() const { return _max_fd; }

	void serveConnections();

    static ServerContext* findServerForHandlingRequest(const std::string& host,
                                                         const int port,
                                                         const std::string& server_name);

    static void routeRequests(const std::string& host, const int port, std::map<int, Request *>& _clients_requests);

	void stop();

    static std::list<ServerContext*> servers_list;

private:
    std::vector<Server*> _servers;

    Config _config;
    // Для заполнения множества сокетов
    fd_set _readset;
    fd_set _writeset;
    int _max_fd;



};


#endif //WEBSERV_WEBSERV_HPP
