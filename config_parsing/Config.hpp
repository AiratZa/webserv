//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include <string>
#include <list>
#include <map>
#include <algorithm>    // std::find

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils/utils.hpp"
#include "../utils/get_next_line/get_next_line.hpp"
#include "../utils/cpp_libft/libft.hpp"
#include "../utils/cpp_libft/stl_containers_operator_overloading.cpp"

// NGINX CONF KEYWORDS (KW) BEGIN

#define SERVER_KW "server"
#define LOCATION_KW "location"

#define LISTEN_KW "listen"
#define SERVER_NAME_KW "server_name"
#define ERROR_PAGE_KW "error_page"
#define CLIENT_MAX_BODY_SIZE_KW "client_max_body_size"
#define LIMIT_EXCEPT_KW "limit_except"
#define ALIAS_KW "alias"
#define AUTOINDEX_KW "autoindex"
#define INDEX_KW "index"

// NGINX CONF KEYWORDS (KW) END




class Config {
public:
    Config() { }
    Config(const std::string &path_to_config);

    class BadConfigException : public std::exception {

    };

private:
    const std::string& _getConfigText(void) const;
    void fillConfigTextFromFile(const std::string &path_to_config);
    void splitConfigTextIntoBlocks(void);

    void parseInsideServerContext(void);
    void parseInsideLocationContext(void);

    std::list<std::string> parseMultipleParamDirective(const std::string &keyword);
    std::list<std::string> parseSingleParamDirective(const std::string &keyword);


    void _fillAllowedContextForKeyWords(void);


    bool _skipSpacesInConfig();



    std::string _config_text;
    int _len;

    void _badConfigError(const std::string & error_text) const;

    bool _is_eof_reached;
    int _tmp_len;

    std::list<std::string> _serverContext;
    std::list<std::string> _locationContext;
    std::list<std::string>::const_iterator _ite_server;
    std::list<std::string>::const_iterator _ite_location;

    std::map<std::string, bool> _isMultipleParamDirective;

};

#endif //WEBSERV_CONFIG_HPP
