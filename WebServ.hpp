//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include <queue>
#include <list>
#include "Server.hpp"

#define CONFIG_FILE_DEFAULT_PATH "/webserv.conf"

class WebServ {

public:
    WebServ(const std::string& config_file_path);
    void addServer(const Server& server);
    int getServersCount(void) const { return _servers.size(); }
    Server& getFrontServer(void);
    void popFrontServer(void);
//    Server& getServerByPosition(int i);
//    void addServersListenerFD(void);

private:
    WebServ() { };
    std::queue<Server> _servers;
//    std::queue<int> _serversListenerFD;


};


#endif //WEBSERV_WEBSERV_HPP
