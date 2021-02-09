//
// Created by airat on 09.02.2021.
//

#include "ServerContext.hpp"

const std::map<std::string, std::list<int> >&  ServerContext::getHostsAndPorts(void) const {
    return _host_port;
}

const std::list<std::string>& ServerContext::getServerNames(void) const {
    return _server_names;
}

LocationContext* ServerContext::addLocation(const std::string& uri) {
    LocationContext* tmp = new LocationContext(uri);
    return tmp;
}

void ServerContext::addHostPort(const std::string& host, int port) {
    if (_host_port.find(host) == _host_port.end()) {
        std::list<int> tmp_list;
        tmp_list.push_back(port);
        _host_port[host] = tmp_list;
    } else {
        _host_port[host].push_back(port);
    }
}

void ServerContext::addServerName(const std::string& server_name) {
    _server_names.push_back(server_name);
}



