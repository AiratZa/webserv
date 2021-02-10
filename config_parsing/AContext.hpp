//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_ACONTEXT_HPP
#define WEBSERV_ACONTEXT_HPP

#include <list>
#include <map>
#include <string>
#include "../utils/cpp_libft/libft.hpp"
#include "../utils/custom_structures/Pair.hpp"

class AContext {
public:
    AContext() { (void)_client_max_body_size; (void)_autoindex;};//TODO: DELETE AFTER TESTS
    virtual ~AContext(void) { };
    virtual void setHostsAndPorts(Pair<std::string, std::list<int> >& host_and_ports) { (void)host_and_ports; };

private:
    std::map<std::string, std::list<std::string> > _error_page; // error_codes, response_code, redirect_uri
    int _client_max_body_size; // converted in bytes;
    bool _autoindex;
    std::list<std::string> _index_pages;

};

#endif //WEBSERV_ACONTEXT_HPP
