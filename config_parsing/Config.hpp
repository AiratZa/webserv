//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include <string>
#include <list>
#include <map>
#include <algorithm>    // std::find

#include <netinet/in.h>
#include <arpa/inet.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils/utils.hpp"
#include "../utils/get_next_line/get_next_line.hpp"
#include "../utils/cpp_libft/libft.hpp"
#include "../utils/cpp_libft/stl_containers_operator_overloading.cpp"

#include "ServerContext.hpp"

//!!! listen *:80 | *:8000;
#define DEFAULT_PORT -1

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
#define ROOT_KW "root"

#define PARAM_CGI_SCRIPT "cgi_script"
#define PARAM_CGI_EXTENSION "cgi_ext"

#define ERROR_PAGE_REDIRECT_URI "ERROR_PAGE_REDIRECT_URI"
#define ERROR_PAGE_CHANGE_ERROR_CODE "ERROR_PAGE_CHANGE_ERROR_CODE"

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

// NGINX CONF KEYWORDS (KW) END




class Config {

// LOCATION CONTEXT FRIEND TEST CLASSES BEGINS

    //Negative scenarios
    FRIEND_TEST(LocationContextTests, innvalid_location_modifier_neg);
    FRIEND_TEST(LocationContextTests, invalid_number_of_arguments_neg);

    //Positive scenarios
    FRIEND_TEST(LocationContextTests, positive_scenarios);

// LOCATION CONTEXT FRIEND TEST CLASSES ENDS




// LISTEN DIRECTIVE FRIEND TEST CLASSES BEGINS

    //Negative scenarios
    FRIEND_TEST(ListenDirectiveHostTests, localhost_not_from_start);
    FRIEND_TEST(ListenDirectiveHostTests, ip_with_negative_number);
    FRIEND_TEST(ListenDirectiveHostTests, ip_with_max_octet_plus_1);
    FRIEND_TEST(ListenDirectiveHostTests, ip_with_more_4_octet);
    FRIEND_TEST(ListenDirectiveHostTests, ip_starts_not_from_digit);

    FRIEND_TEST(ListenDirectivePortsTests, port_min_minus_1);
    FRIEND_TEST(ListenDirectivePortsTests, port_negative);
    FRIEND_TEST(ListenDirectivePortsTests, too_long);
    FRIEND_TEST(ListenDirectivePortsTests, empty_str);
    FRIEND_TEST(ListenDirectivePortsTests, max_plus_1);
    FRIEND_TEST(ListenDirectivePortsTests, with_unexpected_chars_start);
    FRIEND_TEST(ListenDirectivePortsTests, with_unexpected_chars_end);

    FRIEND_TEST(ListenDirectiveHOST_PORTTests, asterisk_plus_port_neg);
    FRIEND_TEST(ListenDirectiveHOST_PORTTests, localhost_or_ip_wtihout_port_neg);
    FRIEND_TEST(ListenDirectiveHOST_PORTTests, just_port_neg);
    FRIEND_TEST(ListenDirectiveHOST_PORTTests, host_port_combo_neg);


    //Positive scenarios
    FRIEND_TEST(ListenDirectiveHostTests, localhost);
    FRIEND_TEST(ListenDirectiveHostTests, min_ip);
    FRIEND_TEST(ListenDirectiveHostTests, max_ip);

    FRIEND_TEST(ListenDirectivePortsTests, min);
    FRIEND_TEST(ListenDirectivePortsTests, max);

    FRIEND_TEST(ListenDirectiveHOST_PORTTests, asterisk_plus_port_pos);
    FRIEND_TEST(ListenDirectiveHOST_PORTTests, localhost_or_ip_wtihout_port_pos);
    FRIEND_TEST(ListenDirectiveHOST_PORTTests, just_port_pos);
    FRIEND_TEST(ListenDirectiveHOST_PORTTests, host_port_combo_pos);

// LISTEN DIRECTIVE FRIEND TEST CLASSES ENDS


// SERVER_NAME DIRECTIVE FRIEND TEST CLASSES BEGINS

    //Negative scenarios
    FRIEND_TEST(ServerNameDirectiveTests, more_than_1_asterisk);
    FRIEND_TEST(ServerNameDirectiveTests, quotes_tests);

    //Positive scenarios
    FRIEND_TEST(ServerNameDirectiveTests, one_server_names_in_one_line);
    FRIEND_TEST(ServerNameDirectiveTests, multiple_server_names_in_one_line);
    FRIEND_TEST(ServerNameDirectiveTests, multiple_server_names_in_multiple_line);

// SERVER_NAME DIRECTIVE FRIEND TEST CLASSES ENDS


// ERROR_PAGE DIRECTIVE FRIEND TEST CLASSES BEGINS
    //// ERROR_PAGE TESTS ARE NOT NEEDED FRIENDSHIP :D
// ERROR_PAGE DIRECTIVE FRIEND TEST CLASSES ENDS


// INDEX DIRECTIVE FRIEND TEST CLASSES BEGINS
    //// ERROR_PAGE TESTS ARE NOT NEEDED FRIENDSHIP :D
// INDEX DIRECTIVE FRIEND TEST CLASSES ENDS







public:
    Config() { }
    Config(const std::string &path_to_config);

    std::list<ServerContext*>& getServersList(void);

    class BadConfigException : public std::exception {

    };

    static void _badConfigError(const std::string & error_text) {
        std::cerr << error_text << std::endl;
        throw Config::BadConfigException();
    }



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


    void _checkAndSetParams(ServerContext* current_server, AContext* current_context, const std::string& directive_keyword,
                            const std::list<std::string>& directive_params);


    // SIGNLE PART CONFIG CHECKS
    std::list<std::string>  _locationKeywordHandler(const std::list<std::string>& context_params) const;

    const Pair<std::string, int > _listenKeywordHandler(const std::list<std::string>& directive_params);
    std::list<std::string> _serverNameKeywordHandler(const std::list<std::string>& directive_params);
    std::map<int, std::map<std::string, std::string> >  _errorPageKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    unsigned long long _clientMaxBodySizeKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    std::list<std::string> _limitExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    std::string _aliasKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    bool _autoindexExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    std::list<std::string> _indexKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    std::string _rootKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    std::string _cgiScriptParamKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);
    std::list<std::string> _cgiExtensionsParamKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params);


    const std::string parseHost(const std::string& param) const;
    int parsePort(const std::string& param) const;

    bool is_correct_serv_name(const std::string& serv_name) const;
    std::string checkAndRemoveQuotes(const std::string& param) const;
    std::size_t parse_until_quote_be_closed(const std::string& serv_name, std::size_t tmp_pos, char found_quote) const;
    void find_first_occured_quote(const std::string& param, std::size_t pos_to_start_search,
                                  int *found_pos, char *found_quote) const;


    const std::string _checkForChangeErrorCodeParam(const std::list<std::string>& directive_params) const;
    int _checkErrorCodeThatShouldBeChanged(const std::string& error_code_str) const;




    std::string _config_text;
    int _len;


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
