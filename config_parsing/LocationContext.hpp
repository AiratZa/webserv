//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_LOCATIONCONTEXT_HPP
#define WEBSERV_LOCATIONCONTEXT_HPP

#include "AContext.hpp"

class LocationContext : public AContext {
public:
    LocationContext(const std::string& uri);

private:
    LocationContext() { };
    std::string _uri;

};


#endif //WEBSERV_LOCATIONCONTEXT_HPP
