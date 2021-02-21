//
// Created by airat on 04.02.2021.
//

#include "Server.hpp"

Server::Server(ServerContext* server_context)
                : server_context(server_context) {
    const std::map<std::string, std::list<int> >& hosts_n_ports = server_context->getHostsAndPorts();

    std::map<std::string, std::list<int> >::const_iterator map_it = hosts_n_ports.begin();
    std::map<std::string, std::list<int> >::const_iterator map_ite = hosts_n_ports.end();

    while (map_it != map_ite) {
        const std::list<int>& ports = (*map_it).second;
        std::list<int>::const_iterator ports_it = ports.begin();
        std::list<int>::const_iterator ports_ite = ports.end();

        // listener initialization for every host:port combination
        while (ports_it != ports_ite) {
            Server::Listener *listener_obj = new Server::Listener((*map_it).first, *ports_it);
            _listeners.push_back(listener_obj);
            ++ports_it;
        }
        ++map_it;
    }
}




















