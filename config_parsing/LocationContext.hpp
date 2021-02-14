//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_LOCATIONCONTEXT_HPP
#define WEBSERV_LOCATIONCONTEXT_HPP

#include "AContext.hpp"
#include "../utils/utils.hpp"

class LocationContext : public AContext {
public:
    LocationContext(const std::string& uri);

    virtual void setLocationUri() { };

    virtual void setErrorPageDirectiveInfo(const std::map<int, std::map<std::string, std::string> >& error_page_info);


private:
    LocationContext() { };
    std::string _uri;

};


#endif //WEBSERV_LOCATIONCONTEXT_HPP
