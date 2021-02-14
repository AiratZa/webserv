//
// Created by airat on 09.02.2021.
//

#include "ServerContext.hpp"
//#include "ServerContext.hpp"

const std::map<std::string, std::list<int> >&  ServerContext::getHostsAndPorts(void) const {
    return _hosts_ports;
}

const std::list<std::string>& ServerContext::getServerNames(void) const {
    return _server_names;
}

LocationContext* ServerContext::addLocation(const std::string& uri) {
    LocationContext* tmp = new LocationContext(uri);
    return tmp;
}

void ServerContext::addHostPort(const std::string& host, int port) {
    if (_hosts_ports.find(host) == _hosts_ports.end()) {
        std::list<int> tmp_list;
        tmp_list.push_back(port);
        _hosts_ports[host] = tmp_list;
    } else {
        _hosts_ports[host].push_back(port);
    }
}

void ServerContext::addServerNames(std::list<std::string>& server_names) {
    std::list<std::string>::iterator it = server_names.begin();

    while (it != server_names.end()) {
        std::list<std::string>::iterator tmp_it;

        std::list<std::string>::iterator ite_private = _server_names.end();
        tmp_it = std::find(_server_names.begin(), ite_private, *it);

        if (tmp_it == ite_private) { // word is not keyword
            _server_names.push_back(*it);
        }
        ++it;
    }
}



