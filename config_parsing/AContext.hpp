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
#include "../utils/utils.hpp"

class AContext {
public:
    AContext() : _client_max_body_size(1024*1024), _is_client_max_body_size_already_set(false) { (void)_autoindex;};//TODO: DELETE AFTER TESTS
    virtual ~AContext(void) { };

    virtual void setErrorPageDirectiveInfo(const std::map<int, std::map<std::string, std::string> >& error_page_info) {
        std::map<int, std::map<std::string, std::string> >::const_iterator it = error_page_info.begin();

        while (it != error_page_info.end()) {
            if (utils::is_exists_in_container(_error_pages_info, (*it).first)) {
                ++it;
            } else {
                addErrorPage((*it).first, (*it).second);
                ++it;
            }
        }
    }

    virtual void setIndexDirectiveInfo(std::list<std::string>& index_paths) {
        _index_pages.splice(_index_pages.end(), index_paths);
    }


    virtual const std::map<int, std::map<std::string, std::string> >& getErrorPagesDirectiveInfo(void) const { return _error_pages_info; }

    virtual const std::list<std::string>& getIndexPagesDirectiveInfo(void) const { return _index_pages; }

    virtual bool setClientMaxBodySize(long long bytes) {
        if (_is_client_max_body_size_already_set)
            return false;
        _client_max_body_size = bytes;
        _is_client_max_body_size_already_set = true;
        return true;
    }

    virtual long long getClientMaxBodySizeInfo(void) const { return _client_max_body_size; }

protected:

    virtual void addErrorPage(int error_code, const std::map<std::string, std::string>& error_page_info) {
        _error_pages_info[error_code] = error_page_info;
    }

    std::map<std::string, std::list<std::string> > _error_page; // error_codes, response_code, redirect_uri
    long long _client_max_body_size; // converted in bytes;
    bool _is_client_max_body_size_already_set;
    bool _autoindex;
    std::list<std::string> _index_pages;

    std::map<int, std::map<std::string, std::string> > _error_pages_info;


};

#endif //WEBSERV_ACONTEXT_HPP
