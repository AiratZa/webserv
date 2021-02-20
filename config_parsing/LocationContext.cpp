//
// Created by airat on 09.02.2021.
//

#include "LocationContext.hpp"

LocationContext::LocationContext(const std::list<std::string>& location_uri_params, const ServerContext& serv_context) {
    _is_exact = (location_uri_params.size() == 2);
    _uri = location_uri_params.back();

    _error_pages_info = serv_context.getErrorPagesDirectiveInfo();
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

