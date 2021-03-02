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
    _autoindex = serv_context.isAutoindexEnabled();
    _root = serv_context.getRootPath();

}

const std::string LocationContext::getLocationPath(void) const {
    return _uri;
}

const std::string LocationContext::getLocationPathForComparison(void) const {
    if (_is_exact)
        return "= " + _uri;
    return _uri;
}

bool LocationContext::setRootPath(const std::string& root_path) {
    if (!_alias_path.empty()) {
        Config::_badConfigError("\"root\" directive is duplicate, \"alias\" directive was specified earlier");
    }
    return AContext::setRootPath(root_path);
}

void LocationContext::setErrorPageDirectiveInfo(const std::map<int, std::map<std::string, std::string> >& error_page_info) {
    if (!_is_error_pages_info_was_updated) {
        _is_error_pages_info_was_updated = true;
        _error_pages_info.clear();
    }

    AContext::setErrorPageDirectiveInfo(error_page_info);
}


bool LocationContext::setLimitedMethodsInfo(const std::list<std::string>& limited_methods) {
    if (!_limit_except_methods.empty()) {
        return false;
    }
    _limit_except_methods = limited_methods;
    return true;
}

bool LocationContext::setAliasPath(const std::string& alias) {
    if (_is_root_already_set) {
        Config::_badConfigError("\"alias\" directive is duplicate, \"root\" directive was specified earlier");
    }

    if (_is_alias_path_already_was_set) {
        return false;
    }
    _alias_path = alias;
    _is_alias_path_already_was_set = true;
    return true;
}

bool LocationContext::setCgiParam(const Pair<std::string, std::string>& cgi_param) {
    _is_cgi_location = true;

    if (cgi_param.first == CGI_PARAM_SCRIPT_NAME) {
        if (!_cgi_script_name.size()) {
            _cgi_script_name = cgi_param.second;
            return true;
        }
    }
    else if (cgi_param.first == CGI_PARAM_PATH_INFO) {
        if (!_cgi_path_info.size()) {
            _cgi_path_info = cgi_param.second;
            return true;
        }
    }
    return false;
}

const std::string LocationContext::getCgiParamByName(const std::string& key) const {
    if (key == CGI_PARAM_SCRIPT_NAME)
    {
        return _cgi_script_name;
    }
    else if (key == CGI_PARAM_PATH_INFO)
    {
        return _cgi_path_info;
    }
    return std::string();
}
