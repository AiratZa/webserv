//
// Created by airat on 09.02.2021.
//

#include "ServerContext.hpp"

const std::list<LocationContext*>& ServerContext::R_getExactLocationsList(void) const {
    return _exact_locations;
}

const std::list<LocationContext*>& ServerContext::R_getNonExactLocationsList(void) const {
    return _non_exact_locations;
}

const std::list<LocationContext*>& ServerContext::R_getExtensionLocationsList(void) const {
    return _ext_locations;
}

void ServerContext::_divideLocationExactNonExactExt(void) {
    std::list<LocationContext*>::const_iterator it = _locations.begin();
    while (it != _locations.end()) {
        if ((*it)->isExact()) {
            _exact_locations.push_back(*it);
        }
        else if ((*it)->isExtLocation()) {
            _ext_locations.push_back(*it);
        }
        else {
            _non_exact_locations.push_back(*it);
        }
        ++it;
    }
}


void ServerContext::structure_properties(void) {
    _divideLocationExactNonExactExt();
}




const std::map<std::string, std::list<int> >&  ServerContext::getHostsAndPorts(void) const {
    return _hosts_ports;
}

const std::list<std::string>& ServerContext::getServerNames(void) const {
    return _server_names;
}

bool ServerContext::_is_location_exist(const std::list<std::string>& location_uri_params) {
    std::string str_for_search;

    bool is_ext_loc = false;

    if ((location_uri_params.size() == 2) && (location_uri_params.front() == "ext")) {
        is_ext_loc = true;
    }

    const std::list<LocationContext*>& locations = this->getLocationsList();

    std::list<LocationContext*>::const_iterator it_l = locations.begin();
    while (it_l != locations.end()) {

        if ((*it_l)->isExtLocation() == is_ext_loc) {

            if (is_ext_loc)
            {
                if (location_uri_params.back() == (*it_l)->getLocationExtension()) {
                    return true;
                }
            }
            else
            {
                if ((*it_l)->getLocationPath() == location_uri_params.back())
                {
                    if (((location_uri_params.size() == 2) && ((*it_l)->isExact())) ||
                            ((location_uri_params.size() == 1) && !((*it_l)->isExact()))) {
                        return true;
                    }
                }
            }
        }

        ++it_l;
    }
    return false;
}


LocationContext* ServerContext::addLocation(const std::list<std::string>& location_uri_params) {
    if (_is_location_exist(location_uri_params))
        return NULL;
    LocationContext* tmp = new LocationContext(location_uri_params, *this);
    _locations.push_back(tmp);
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





