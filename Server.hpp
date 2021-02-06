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

#include "utils.hpp"
#include "request_handlers/Request.hpp"

class Server {
public:
    Server(int port);
    ~Server() { };

    const int & getListener(void) const { return _listener; };
    fd_set* getReadSetPtr(void) { return &_readset; };
    fd_set* getWriteSetPtr(void) { return &_writeset; };

    void setToReadFDSet(void);
    void setToWriteFDSet(void);
    void updateMaxFD(void) { _maxFD = std::max(std::max(_listener,
                                              *std::max_element(_clients_read.begin(), _clients_read.end())),
                                              *std::max_element(_clients_write.begin(), _clients_write.end()) );
    }

    const int & getMaxFD() const { return _maxFD; }

    void acceptConnection(void);
    void processConnections(void);
    void handleRequests(void);
    void handleResponses(void);
    

private:
    int _listener;
    struct sockaddr_in _addr;
    int _port;

    std::list<int> _clients_read;
    std::list<int> _clients_write;

    std::map<int, MyRequest> _client_requests;

    // Для заполнения множества сокетов
    fd_set _readset;
    fd_set _writeset;

    int _maxFD;


    Server() { };


};

#endif //WEBSERV_SERVER_HPP
