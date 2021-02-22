//
// Created by airat on 04.02.2021.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <list>
#include "config_parsing/ServerContext.hpp"
#include "Listener.hpp"

//#include <map>
//
//#include <algorithm>
//#include <sstream>
//
//#include <sys/socket.h>
//#include <sys/time.h>
//#include <netinet/in.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <arpa/inet.h>
//
//#include "utils/utils.hpp"
//#include "request_handlers/Request.hpp"
//
//#include "utils/cpp_libft/libft.hpp"
//
//#include "response/Response.hpp"



#define BUFFER_LENGHT 1024
#define TIME_OUT 15


class Server {
public:
    Server(ServerContext* server_context);
    ~Server() { };

    const std::list<Listener*> & getListeners(void) const { return _listeners; };


    Server() { };
private:
    std::list<Listener*> _listeners;
//    ServerContext* server_context;
};

#endif //WEBSERV_SERVER_HPP
