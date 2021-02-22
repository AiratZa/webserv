//
// Created by airat on 21.02.2021.
//

#include "WebServ.hpp"

//// https://nginx.org/ru/docs/http/request_processing.html

/*
 * При поиске виртуального сервера по имени, если имени соответствует несколько из указанных вариантов,
 * например, одновременно подходят и имя с маской, и регулярное выражение,
 * будет выбран первый подходящий вариант в следующем порядке приоритета:
 * 1 точное имя
 * 2 самое длинное имя с маской в начале, например “*.example.com”
 * 3 самое длинное имя с маской в конце, например “mail.*”
 * =================================================================================
 * returns 0 if match is not found
 * returns 1 if FULL match
 * returns 2 if match with mask at start
 * returns 3 if match with mask at end
 */

int isServerNameMatch(const std::string& server_name, const std::list<std::string>& serv_name_list) {
    std::list<std::string>::const_iterator it = serv_name_list.begin();
    std::size_t found_pos;

    int value_to_return = 0;

    while (it != serv_name_list.end()) {
        if (server_name == *it) {
            return 1;
        } else if ((*it)[0] == '*'){
            found_pos = server_name.find_first_of('.');
            if (found_pos != std::string::npos) {
                if (server_name.substr(found_pos+1) == (*it).substr(2))
                {
                    if ((value_to_return > 2) || (!value_to_return))
                        value_to_return = 2;
                }
            }
        } else if ((*it)[(*it).size()-1] == '*') {
            found_pos = server_name.find_last_of('.');
            if (found_pos != std::string::npos) {
                if (server_name.substr(0, found_pos-1) == (*it).substr(0, (*it).size()-3))
                {
                    if ((value_to_return > 3) || (!value_to_return))
                        value_to_return = 3;
                }
            }
        }
        ++it;
    }
    return value_to_return;
}


ServerContext* WebServ::findServerForHandlingRequest(const std::string& host,
                                                                const int port,
                                                                const std::string& server_name) {
    std::list<ServerContext*>::const_iterator it = servers_list.begin();
    ServerContext* default_serv = NULL;

    std::list<ServerContext*> match_with_mask_at_start;
    std::list<ServerContext*> match_with_mask_at_end;

    int tmp = 0;

    while(it != servers_list.end()) {
        const std::list<std::string>& serv_name_list = (*it)->getServerNames();

        const std::map<std::string, std::list<int> >& host_n_ports = (*it)->getHostsAndPorts();
        std::map<std::string, std::list<int> >::const_iterator it_h_p = host_n_ports.begin();

        while(it_h_p != host_n_ports.end()) {
            if (host == (*it_h_p).first) {
                std::list<int>::const_iterator it_p = (*it_h_p).second.begin();
                while (it_p != (*it_h_p).second.end()) {
                    if (port == (*it_p)) {
                        if ((tmp = isServerNameMatch(server_name, serv_name_list))) {
                            if (tmp == 1) {
                                return *it; // full match
                            } else if (tmp == 2) {
                                match_with_mask_at_start.push_back(*it);
                            } else if (tmp == 3) {
                                match_with_mask_at_end.push_back(*it);
                            }
                        } else { // set default if it's NULL and find full match
                            if (!default_serv)
                                default_serv = *it;
                        }
                    }
                    ++it_p;
                }
            }
            ++it_h_p;
        }
        ++it;
    }
    if (!match_with_mask_at_start.empty())
        return match_with_mask_at_start.front();
    if (!match_with_mask_at_end.empty())
        return match_with_mask_at_end.front();
    return default_serv;
}

LocationContext* searchForBestMatchLocation(ServerContext* handling_server, Request* current_request) {
    const std::list<LocationContext*>& exact = handling_server->R_getExactLocationsList();
    std::list<LocationContext*>::const_iterator it_exact = exact.begin();
    while (it_exact != exact.end()) {
        if ((*it_exact)->getLocationPath() == current_request->_request_target) {
            return (*it_exact); // exact route(location) is found
        }
        ++it_exact;
    }

    // Searching in Non Exact Locations
    const std::list<LocationContext*>& non_exact = handling_server->R_getNonExactLocationsList();
    std::size_t target_len = current_request->_request_target.size();

    while (target_len > 1) {
        std::string target_substr = current_request->_request_target.substr(0, (target_len-1));

        std::list<LocationContext*>::const_iterator it_non_exact = non_exact.begin();
        while (it_non_exact != non_exact.end()) {
            if ((*it_non_exact)->getLocationPath() == target_substr) {
                return (*it_non_exact); // exact route(location) is found
            }
            ++it_non_exact;
        }

        target_len--;
    }
    return NULL; // Not match to locations (404 Not Found)
}

void WebServ::routeRequests(const std::string& host, const int port, std::map<int, Request *>& _clients_requests) {
    std::map<int, Request *>::iterator it_r = _clients_requests.begin();

    while (it_r != _clients_requests.end()) {
        Request* current_request = (*it_r).second;
        std::string host_from_header = current_request->_headers["host"];

        ServerContext* handling_server = findServerForHandlingRequest(host, port, host_from_header);
        current_request->setHandlingServer(handling_server);

        LocationContext* location_to_route = searchForBestMatchLocation(handling_server, current_request);
        current_request->setHandlingLocation(location_to_route);

        ++it_r;
    }
}
