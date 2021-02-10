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

#include "ServerContext.hpp"


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

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

// NGINX CONF KEYWORDS (KW) END




class Config {
    FRIEND_TEST(ListenDirectiveTests, testOne);

public:
    Config() { }
    Config(const std::string &path_to_config);

    const std::list<ServerContext*>& getServersList(void) const;

    class BadConfigException : public std::exception {

    };



private:
    const std::string& _getConfigText(void) const;
    void fillConfigTextFromFile(const std::string &path_to_config);
    void splitConfigTextIntoBlocks(void);

    void parseInsideServerContext(ServerContext* current_server);
    void parseInsideLocationContext(ServerContext* current_server);

    std::list<std::string> parseMultipleParamDirective(const std::string &keyword);
    std::list<std::string> parseSingleParamDirective(const std::string &keyword);

    void _fillAllowedContextForKeyWords(void);


    bool _skipSpacesInConfig();


    void _checkAndSetParams(AContext* current_context, const std::string& directive_keyword,
                            const std::list<std::string>& directive_params);


    // SIGNLE PART CONFIG CHECKS
    void _locationUriChecks(const std::string& location_uri);
    Pair<std::string, std::list<int> >* _listenKeywordHandler(const std::list<std::string>& directive_params);
    void _serverNameKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    void _errorPageKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    void _clientMaxBodySizeKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    void _limitExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    void _aliasKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    void _autoindexExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    void _indexExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);


    const std::string parseHost(const std::string& param) const;


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

    std::list<ServerContext*> _servers;


};

#endif //WEBSERV_CONFIG_HPP
