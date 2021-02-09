//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_SERVERCONTEXT_HPP
#define WEBSERV_SERVERCONTEXT_HPP

#include <list>
#include <map>
#include "AContext.hpp"
#include "LocationContext.hpp"

class ServerContext : public AContext {

public:
    ServerContext() { };

    const std::map<std::string, std::list<int> >&  getHostsAndPorts(void) const;
    const std::list<std::string>& getServerNames(void) const;

    LocationContext* addLocation(const std::string& uri);

    void addHostPort(const std::string& host, int port);
    void addServerName(const std::string& server_name);

private:
    std::list<LocationContext*> _locations;
    std::map<std::string, std::list<int> > _host_port;
    std::list<std::string> _server_names;

};

#endif //WEBSERV_SERVERCONTEXT_HPP
