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

int WebServ::isServerNameMatch(const std::string& server_name, const std::list<std::string>& serv_name_list) {
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

std::list<ServerContext*> WebServ::getAllExactHostPortComboList(const std::string& host,
                                                                const int port)
{
    std::list<ServerContext*> to_return;

    std::list<ServerContext*>::const_iterator it = servers_list.begin();
    while (it != servers_list.end()) {
        const std::map<std::string, std::list<int> >& host_n_ports = (*it)->getHostsAndPorts();
        std::map<std::string, std::list<int> >::const_iterator it_h_p = host_n_ports.begin();

        while(it_h_p != host_n_ports.end()) {
            if (host == (*it_h_p).first) {
                std::list<int>::const_iterator it_p = (*it_h_p).second.begin();
                while (it_p != (*it_h_p).second.end()) {
                    if (port == (*it_p)) {
                        to_return.push_back(*it);
                    }
                    ++it_p;
                }
            }
            ++it_h_p;
        }
        ++it;
    }
    return to_return;
}

std::list<ServerContext*> WebServ::getAllAsteriskHostPortComboList(const int port)
{
    std::list<ServerContext*> to_return;

    std::list<ServerContext*>::const_iterator it = servers_list.begin();
    while (it != servers_list.end()) {
        const std::map<std::string, std::list<int> >& host_n_ports = (*it)->getHostsAndPorts();
        std::map<std::string, std::list<int> >::const_iterator it_h_p = host_n_ports.begin();

        while(it_h_p != host_n_ports.end()) {
            if ("*" == (*it_h_p).first) {
                std::list<int>::const_iterator it_p = (*it_h_p).second.begin();
                while (it_p != (*it_h_p).second.end()) {
                    if (port == (*it_p)) {
                        to_return.push_back(*it);
                    }
                    ++it_p;
                }
            }
            ++it_h_p;
        }
        ++it;
    }
    return to_return;
}



ServerContext* WebServ::findServerForHandlingRequest(const std::string& host,
                                                                const int port,
                                                                const std::string& server_name) {
    ServerContext* default_serv = NULL;

    std::list<ServerContext*> match_with_mask_at_start;
    std::list<ServerContext*> match_with_mask_at_end;

    int tmp = 0;

    //// #1 EXACT HOST(IP, localhost):PORT COMBO
    std::list<ServerContext*> exact_host_port = getAllExactHostPortComboList(host, port);
    std::list<ServerContext*>::const_iterator it_exact = exact_host_port.begin();

    while(it_exact != exact_host_port.end()) {
        const std::list<std::string>& serv_name_list = (*it_exact)->getServerNames();

        const std::map<std::string, std::list<int> >& host_n_ports = (*it_exact)->getHostsAndPorts();
        std::map<std::string, std::list<int> >::const_iterator it_h_p = host_n_ports.begin();

        while(it_h_p != host_n_ports.end()) {
            if (host == (*it_h_p).first) {
                std::list<int>::const_iterator it_p = (*it_h_p).second.begin();
                while (it_p != (*it_h_p).second.end()) {
                    if (port == (*it_p)) {
                        if ((tmp = isServerNameMatch(server_name, serv_name_list))) {
                            if (tmp == 1) {
                                return *it_exact; // full match
                            } else if (tmp == 2) {
                                match_with_mask_at_start.push_back(*it_exact);
                            } else if (tmp == 3) {
                                match_with_mask_at_end.push_back(*it_exact);
                            }
                        } else { // set default if it's NULL and find full match
                            if (!default_serv)
                                default_serv = *it_exact;
                        }
                    }
                    ++it_p;
                }
            }
            ++it_h_p;
        }
        ++it_exact;
    }
    if (!match_with_mask_at_start.empty())
        return match_with_mask_at_start.front();
    if (!match_with_mask_at_end.empty())
        return match_with_mask_at_end.front();
    if (default_serv)
        return default_serv;


    //// #2 *:PORT COMBO
    std::list<ServerContext*> asterisk_host_port = getAllAsteriskHostPortComboList(port);
    std::list<ServerContext*>::const_iterator it_ast = asterisk_host_port.begin();

    while(it_ast != asterisk_host_port.end()) {
        const std::list<std::string>& serv_name_list = (*it_ast)->getServerNames();

        const std::map<std::string, std::list<int> >& host_n_ports = (*it_ast)->getHostsAndPorts();
        std::map<std::string, std::list<int> >::const_iterator it_h_p = host_n_ports.begin();

        while(it_h_p != host_n_ports.end()) {
            if (host == (*it_h_p).first) {
                std::list<int>::const_iterator it_p = (*it_h_p).second.begin();
                while (it_p != (*it_h_p).second.end()) {
                    if (port == (*it_p)) {
                        if ((tmp = isServerNameMatch(server_name, serv_name_list))) {
                            if (tmp == 1) {
                                return *it_ast; // full match
                            } else if (tmp == 2) {
                                match_with_mask_at_start.push_back(*it_ast);
                            } else if (tmp == 3) {
                                match_with_mask_at_end.push_back(*it_ast);
                            }
                        } else { // set default if it's NULL and find full match
                            if (!default_serv)
                                default_serv = *it_ast;
                        }
                    }
                    ++it_p;
                }
            }
            ++it_h_p;
        }
        ++it_ast;
    }
    if (!match_with_mask_at_start.empty())
        return match_with_mask_at_start.front();
    if (!match_with_mask_at_end.empty())
        return match_with_mask_at_end.front();
    return default_serv;
}


bool WebServ::isPartOfLocationPath(const std::list<std::string>& request_target, const std::list<std::string>& location_path) {
    std::list<std::string>::const_iterator it1 = request_target.begin();
    std::list<std::string>::const_iterator it1_e = request_target.end();

    std::list<std::string>::const_iterator it2 = location_path.begin();
    std::list<std::string>::const_iterator it2_e = location_path.end();

    while((it1 != it1_e) && (it2 != it2_e)) {
        if ((*it1) != (*it2)) {
            return false;
        }
        ++it1;
        ++it2;
    }
    if ((it2 == it2_e))
    	return true;
    return false;
}


LocationContext* WebServ::searchForBestMatchLocation(ServerContext* handling_server,
                                            Request* current_request,
                                            const std::string& request_target) {
    const std::list<LocationContext*>& exact = handling_server->R_getExactLocationsList();
    std::list<LocationContext*>::const_iterator it_exact = exact.begin();
    while (it_exact != exact.end()) {
        if ((*it_exact)->getLocationPath() == request_target) {
            return (*it_exact); // exact route(location) is found
        }
        ++it_exact;
    }
    LocationContext* ext_location = NULL;

    const std::list<LocationContext*>& extension_loc = handling_server->R_getExtensionLocationsList();
    std::list<LocationContext*>::const_iterator it_extension = extension_loc.begin();
    while (it_extension != extension_loc.end()) {
        const std::string& tmp_loc_ext = (*it_extension)->getLocationExtension();
        std::size_t tmp_loc_ext_len = tmp_loc_ext.size();

        if (request_target.size() > tmp_loc_ext_len) {
            std::size_t start_pos = request_target.size() - tmp_loc_ext_len;
            std::string end_of_request_target = request_target.substr(start_pos);

            if (end_of_request_target == tmp_loc_ext) {
                bool not_allowed = current_request->isMethodLimited(*(*it_extension));

                // no limits
                if (!not_allowed) {
                    ext_location = *it_extension;
                    break;
                }
            }
        }
        ++it_extension;
    }
    /*
     * added by jnannie because current_request->_request_target == '/' didnt work
     */
	const std::list<LocationContext*>& non_exact = handling_server->R_getNonExactLocationsList();
	std::list<LocationContext*>::const_iterator it_non_exact = non_exact.begin();
	while (it_non_exact != non_exact.end()) {
		if ((*it_non_exact)->getLocationPath() == current_request->_request_target) {
			if (ext_location) {
				current_request->setCgiScriptPathForRequest(ext_location->getCgiScript());
			}
			return (*it_non_exact); // exact route(location) is found
		}
		++it_non_exact;
	}

    // Searching in Non Exact Locations
    std::list<std::string> request_target_divided = divideURIBySlashSymbols(request_target);
    it_non_exact = non_exact.begin();
    while (it_non_exact != non_exact.end()) {
        bool is_part = isPartOfLocationPath(request_target_divided,(*it_non_exact)->getLocationPathDividedBySlahes());
        if (is_part) {
            if (ext_location) {
				current_request->setCgiScriptPathForRequest(ext_location->getCgiScript());
			}
            return (*it_non_exact); // exact route(location) is found
        }
        ++it_non_exact;
    }
    return NULL; // Not match to locations
}

void WebServ::routeRequest(const std::string& host, const int port, Request* _client_request, const std::string& request_target) {
	std::string tmp_request_target = request_target;
    std::map<std::string, std::string>::const_iterator it = _client_request->_headers.find("host");

    std::string input_host = host;
    libft::string_to_lower(input_host);

    std::string host_from_header;

    if (it != _client_request->_headers.end()) {
        host_from_header = it->second.substr(0, it->second.find(':')); //jnannie: header contains host:port so we must remove port
    } else { // jnannie: header "host" must be, rfc7230 5.4
    	return _client_request->setStatusCode(400);
    }

    libft::string_to_lower(host_from_header);

    ServerContext* handling_server = findServerForHandlingRequest(input_host, port, host_from_header);
    _client_request->setHandlingServer(handling_server);

    LocationContext* location_to_route = searchForBestMatchLocation(handling_server, _client_request, tmp_request_target);
    _client_request->setHandlingLocation(location_to_route);

    _client_request->setAbsoluteRootPathForRequest();
}



