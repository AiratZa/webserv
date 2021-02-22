//
// Created by airat on 21.02.2021.
//

#include "Server.hpp"
#include "WebServ.hpp"

Server::Listener::Listener(const std::string &host, int port)
                : _host(host), _port(port)
{
    _listener = socket(AF_INET, SOCK_STREAM, 0);
    if(_listener < 0)
        utils::exitWithLog();

    if (fcntl(_listener, F_SETFL, O_NONBLOCK) < 0) //превращает сокет в неблокирующий
        utils::exitWithLog();

    int optval = 1;
    if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        utils::exitWithLog();

    in_addr_t host_addr = _getHostInetAddrFromStr(host);

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = host_addr;


    if (bind(_listener, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
        utils::exitWithLog();

    if (listen(_listener, SOMAXCONN) < 0)
        utils::exitWithLog();
}

in_addr_t Server::Listener::_getHostInetAddrFromStr(const std::string& host_str) const {
    in_addr_t host_addr;
    if (host_str == "*") {
        host_addr = htonl(INADDR_ANY);
    } else if (host_str == "localhost") {
        host_addr = htonl(INADDR_LOOPBACK);
    } else {
        host_addr = inet_addr(host_str.c_str());
    }
    return host_addr;
}

void Server::Listener::updateMaxFD(void) {
    int max_tmp = _listener;
    if (!_clients_read.empty()) {
        max_tmp = std::max(max_tmp, *std::max_element(_clients_read.begin(), _clients_read.end()));
    }
    if (!_clients_write.empty()) {
        max_tmp = std::max(max_tmp, *std::max_element(_clients_write.begin(), _clients_write.end()));
    }
    _max_fd = max_tmp;
}



void Server::Listener::processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr) {
    handleRequests(globalReadSetPtr);
    ////Router in developing. Deadline 22.02.2021
//    WebServ::routeRequests(_host, _port, _client_requests);
    handleResponses(globalWriteSetPtr);
}



