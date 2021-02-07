//
// Created by airat on 04.02.2021.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <list>
#include <map>

#include <algorithm>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils/utils.hpp"
#include "request_handlers/Request.hpp"

class Server {
public:
    Server(int port);
    ~Server() { };

    const int & getListener(void) const { return _listener; };

    void updateMaxFD(void);

    const int & getMaxFD() const { return _max_fd; }

    void acceptConnection(void);
    void processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr);
    void handleRequests(fd_set* globalReadSetPtr);
    void handleResponses(fd_set* globalWriteSetPtr);

    std::list<int>& getReadClients(void) { return _clients_read; }
    

private:
    int _listener;
    struct sockaddr_in _addr;
    int _port;

    std::list<int> _clients_read;
    std::list<int> _clients_write;

    std::map<int, MyRequest *> _client_requests;

    int _max_fd;


    Server() { };


};

#endif //WEBSERV_SERVER_HPP
