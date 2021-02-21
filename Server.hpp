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
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "utils/utils.hpp"
#include "request_handlers/Request.hpp"
#include "config_parsing/ServerContext.hpp"
#include "utils/cpp_libft/libft.hpp"

#include "response/Response.hpp"

#define BUFFER_LENGHT 1024
#define TIME_OUT 15


class Server {
public:

    class Listener {

    public:
        Listener(const std::string& host, int port);

        int getListener(void) const { return _listener; }

        const std::list<int>& getReadClients(void) const { return _clients_read; }
        const std::list<int>& getWriteClients(void) const { return _clients_write; }
        void updateMaxFD(void);

        void acceptConnection(void);

        void processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr);
        void handleRequests(fd_set* globalReadSetPtr);
        void handleResponses(fd_set* globalWriteSetPtr);
        void checkRequest(Request* request);

        const int & getMaxFD() const { return _max_fd; }

        //skarry:
        int     checkFullRequest(std::string const& req);
        void    readError(std::list<int>::iterator & it);


    private:
        int _listener;

        struct sockaddr_in _addr;
        const std::string _host;
        const int _port;

        std::list<int> _clients_read;
        std::list<int> _clients_write;
        std::map<int, Request *> _client_requests;

        int _max_fd;

        in_addr_t _getHostInetAddrFromStr(const std::string& host_str) const;
    };


    Server(ServerContext* server_context);
    ~Server() { };

    const std::list<Listener*> & getListeners(void) const { return _listeners; };


    Server() { };
private:
    std::list<Listener*> _listeners;
    ServerContext* server_context;
};

#endif //WEBSERV_SERVER_HPP
