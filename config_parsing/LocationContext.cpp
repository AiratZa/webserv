//
// Created by airat on 09.02.2021.
//

#include "LocationContext.hpp"

LocationContext::LocationContext(const std::list<std::string>& location_uri_params, const ServerContext& serv_context)
                    : _is_error_pages_info_was_updated(false),
                      _alias_path(""),
                      _is_alias_path_already_was_set(false){
    _is_exact = (location_uri_params.size() == 2);
    _uri = location_uri_params.back();

    _error_pages_info = serv_context.getErrorPagesDirectiveInfo();
    _index_pages = serv_context.getIndexPagesDirectiveInfo();
    _client_max_body_size = serv_context.getClientMaxBodySizeInfo();

}

const std::string LocationContext::getLocationPath(void) const {
    if (_is_exact)
        return "= " + _uri;
    return _uri;
}


void LocationContext::setErrorPageDirectiveInfo(const std::map<int, std::map<std::string, std::string> >& error_page_info) {
    if (!_is_error_pages_info_was_updated) {
        _is_error_pages_info_was_updated = true;
        _error_pages_info.clear();
    }

    AContext::setErrorPageDirectiveInfo(error_page_info);
}

void LocationContext::setIndexDirectiveInfo(std::list<std::string>& index_paths) {
    if (!_is_index_pages_info_was_updated) {
        _is_index_pages_info_was_updated = true;
        _index_pages.clear();
    }
    AContext::setIndexDirectiveInfo(index_paths);
}


bool LocationContext::setLimitedMethodsInfo(const std::list<std::string>& limited_methods) {
    if (!_limited_methods.empty()) {
        return false;
    }
    _limited_methods = limited_methods;
    return true;
}

bool LocationContext::setAliasPath(const std::string& alias) {
    if (_is_alias_path_already_was_set) {
        return false;
    }
    _alias_path = alias;
    _is_alias_path_already_was_set = true;
    return true;
}
