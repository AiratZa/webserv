//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_SERVERCONTEXT_HPP
#define WEBSERV_SERVERCONTEXT_HPP

#include <list>
#include <map>
#include <algorithm>

class LocationContext;

#include "AContext.hpp"
#include "LocationContext.hpp"


class ServerContext : public AContext {

public:

    // 'R' stands for router using
    const std::list<LocationContext*>& R_getExactLocationsList(void) const;
    const std::list<LocationContext*>& R_getNonExactLocationsList(void) const;
    const std::list<LocationContext*>& R_getExtensionLocationsList(void) const;



    ServerContext() : is_server_names_were_updated(false) { _server_names.push_back("");  };

    const std::map<std::string, std::list<int> >&  getHostsAndPorts(void) const;
    const std::list<std::string>& getServerNames(void) const;

    bool get_status_is_server_names_were_updated(void) const { return is_server_names_were_updated;}
    void set_server_names_were_updated(bool value) { is_server_names_were_updated = value; }
    void clear_server_names(void) { _server_names.clear(); }


    const std::list<LocationContext*>& getLocationsList(void) const {
        return _locations;
    }

    LocationContext* addLocation(const std::list<std::string>& location_uri_params);

    void addHostPort(const std::string& host, int port);
    void addServerNames(std::list<std::string>& server_names);

    // AFTER PARSING
    void structure_properties(void);

private:
    std::list<LocationContext*> _locations;
    std::map<std::string, std::list<int> > _hosts_ports;
    std::list<std::string> _server_names;

    bool is_server_names_were_updated;

    bool _is_location_exist(const std::list<std::string>& location_uri_params);


    // AFTER PARSING
    std::list<LocationContext*> _exact_locations;
    std::list<LocationContext*> _non_exact_locations;
    std::list<LocationContext*> _ext_locations;


    void _divideLocationExactNonExactExt(void);
};

#endif //WEBSERV_SERVERCONTEXT_HPP
