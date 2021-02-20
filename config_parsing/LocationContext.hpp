//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_LOCATIONCONTEXT_HPP
#define WEBSERV_LOCATIONCONTEXT_HPP

class ServerContext;

#include "AContext.hpp"
#include "ServerContext.hpp"
#include "../utils/utils.hpp"

class LocationContext : public AContext {
public:
    LocationContext(const std::list<std::string>& location_uri_params, const ServerContext& serv_context);

    const std::string getLocationPath(void) const;

    virtual void setLocationUri() { };

    virtual void setErrorPageDirectiveInfo(const std::map<int, std::map<std::string, std::string> >& error_page_info);


private:
    LocationContext() { }
    bool _is_exact; //is contains "=" modifier
    std::string _uri;

};


#endif //WEBSERV_LOCATIONCONTEXT_HPP
