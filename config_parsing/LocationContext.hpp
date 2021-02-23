//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_LOCATIONCONTEXT_HPP
#define WEBSERV_LOCATIONCONTEXT_HPP

class ServerContext;

#include "AContext.hpp"
#include "ServerContext.hpp"
#include "../utils/utils.hpp"
#include "Config.hpp"

class LocationContext : public AContext {
public:
    LocationContext(const std::list<std::string>& location_uri_params, const ServerContext& serv_context);

    bool isExact(void) const { return _is_exact; }
    const std::string getLocationPath(void) const;


    virtual void setErrorPageDirectiveInfo(const std::map<int, std::map<std::string, std::string> >& error_page_info);


    bool setLimitedMethodsInfo(const std::list<std::string>& limited_methods);

    const std::list<std::string>& getLimitExceptMethods(void) const {return _limit_except_methods;};

    bool setAliasPath(const std::string& alias);

    const std::string& getAliasPath(void) const { return _alias_path; }

    const std::string getLocationPathForComparison(void) const;

    virtual bool setRootPath(const std::string& root_path);

private:


    LocationContext() { }
    bool _is_exact; //is contains "=" modifier
    std::string _uri;

    bool _is_error_pages_info_was_updated;

    std::list<std::string> _limit_except_methods;

    std::string _alias_path;
    bool _is_alias_path_already_was_set;


};


#endif //WEBSERV_LOCATIONCONTEXT_HPP
