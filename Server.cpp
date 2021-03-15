//
// Created by airat on 04.02.2021.
//

#include "Server.hpp"

Server::~Server() {
    std::list<Listener*>::iterator _listeners_it = _listeners.begin();
    while (_listeners_it != _listeners.end()) {
        delete *_listeners_it;
        ++_listeners_it;
    }
}

in_addr_t getHostInetAddrFromStr(const std::string& host_str) {
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

Server::Server(ServerContext* server_context) {
    const std::map<std::string, std::list<int> >& hosts_n_ports = server_context->getHostsAndPorts();

    std::map<std::string, std::list<int> >::const_iterator map_it = hosts_n_ports.begin();
    std::map<std::string, std::list<int> >::const_iterator map_ite = hosts_n_ports.end();

    while (map_it != map_ite) {
        std::string host_str = (*map_it).first;
        const std::list<int>& ports = (*map_it).second;
        if (host_str == "localhost") {
            host_str = "127.0.0.1";
        }
        std::list<int>::const_iterator ports_it = ports.begin();
        std::list<int>::const_iterator ports_ite = ports.end();

        // listener initialization for every host:port combination
        while (ports_it != ports_ite) {
            if (WebServ::isAlreadyListeningHostPlusPort(host_str, *ports_it))
            {
                ++ports_it; // dont rise up same host+port combo again
            }
            else
            {
                in_addr_t host_addr = getHostInetAddrFromStr((*map_it).first);
                Listener *listener_obj = new Listener(host_str, host_addr, *ports_it);
                _listeners.push_back(listener_obj);
                WebServ::setInAlreadyListeningHostPlusPort(host_str, *ports_it);
                ++ports_it;
            }
        }
        ++map_it;
    }
}
