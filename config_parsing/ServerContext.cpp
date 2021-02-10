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

void ServerContext::setHostsAndPorts(Pair<std::string, std::list<int> >& host_and_ports) {
    _hosts_ports[host_and_ports.first] = host_and_ports.second;
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

void ServerContext::addServerName(const std::string& server_name) {
    _server_names.push_back(server_name);
}



