//
// Created by airat on 04.02.2021.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

class Server;
#include <list>
#include "../config_parsing/ServerContext.hpp"
#include "Listener.hpp"

class Server {
public:
    Server(){}
    Server(ServerContext* server_context);
    ~Server();
    const std::list<Listener*> & getListeners(void) const { return _listeners; };
private:
    std::list<Listener*> _listeners;
};

#endif //WEBSERV_SERVER_HPP
