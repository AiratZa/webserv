//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_ACONTEXT_HPP
#define WEBSERV_ACONTEXT_HPP

#include <list>
#include <map>
#include <string>

class AContext {
public:
    AContext() { (void)_client_max_body_size; (void)_autoindex;};//TODO: DELETE AFTER TESTS
    virtual ~AContext(void) { };

private:
    std::map<std::string, std::list<std::string> > _error_page; // error_codes, response_code, redirect_uri
    int _client_max_body_size; // converted in bytes;
    bool _autoindex;
    std::list<std::string> _index_pages;

};

#endif //WEBSERV_ACONTEXT_HPP
