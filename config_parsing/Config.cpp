//
// Created by airat on 06.02.2021.
//

#include "Config.hpp"

Config::Config(const std::string &path_to_config) {
    _fillAllowedContextForKeyWords();
    fillConfigTextFromFile(path_to_config);
    _len = _config_text.size();
    splitConfigTextIntoBlocks();
}

const std::string& Config::_getConfigText(void) const {
    return _config_text;
}

const std::list<ServerContext*>& Config::getServersList(void) const {
    return _servers;
}

void Config::_fillAllowedContextForKeyWords(void) {
    // SERVER CONTEXT
    _serverContext.push_back(LOCATION_KW); //!!! CONTEXT !!!

    _serverContext.push_back(LISTEN_KW);
    _serverContext.push_back(SERVER_NAME_KW);
    _serverContext.push_back(ERROR_PAGE_KW); //UNIVERSAL
    _serverContext.push_back(CLIENT_MAX_BODY_SIZE_KW); //UNIVERSAL
    _serverContext.push_back(AUTOINDEX_KW); //UNIVERSAL
    _serverContext.push_back(INDEX_KW); //UNIVERSAL



    //LOCATION CONTEXT
    _locationContext.push_back(ERROR_PAGE_KW); //UNIVERSAL
    _locationContext.push_back(CLIENT_MAX_BODY_SIZE_KW); //UNIVERSAL
    _locationContext.push_back(LIMIT_EXCEPT_KW);
    _locationContext.push_back(ALIAS_KW);
    _locationContext.push_back(AUTOINDEX_KW); //UNIVERSAL
    _locationContext.push_back(INDEX_KW); //UNIVERSAL


    _ite_server = _serverContext.end();
    _ite_location = _locationContext.end();

    _isMultipleParamDirective[SERVER_NAME_KW] = true;
    _isMultipleParamDirective[ERROR_PAGE_KW] = true;
    _isMultipleParamDirective[INDEX_KW] = true;
    _isMultipleParamDirective[LIMIT_EXCEPT_KW] = true;

    _isMultipleParamDirective[LISTEN_KW] = false;
    _isMultipleParamDirective[CLIENT_MAX_BODY_SIZE_KW] = false;
    _isMultipleParamDirective[AUTOINDEX_KW] = false;
    _isMultipleParamDirective[ALIAS_KW] = false;


}



void Config::fillConfigTextFromFile(const std::string &path_to_config) {
    int file = open(path_to_config.c_str(), O_RDONLY);
    if (file < 0)
        utils::exitWithLog("Error happened when try to open config file :(");

    char *str;
    int ret;

    while ((ret = get_next_line(file, &str)) == 1) {
        _config_text += str;
        delete str;
    }
    _config_text += str;
    delete str;

    if (ret != 0) {
        utils::exitWithLog("Error happened when read config file :(");
    }
    close(file);
}



/*
 * return true if no more words
 * return true to continue parsing
 */
bool Config::_skipSpacesInConfig() {
    const std::string& const_config_text = _getConfigText();

    while(libft::isspace(const_config_text[_tmp_len]) && (_tmp_len < _len)) {
        ++_tmp_len;
    }
    if (_tmp_len == _len) {
        return true;
    }
    return false;
}


void Config::splitConfigTextIntoBlocks(void) {
    const std::string& const_config_text = _getConfigText();
    _tmp_len = 0; //initialize tmp value (start is here)

    std::string tmpWord;

    while (TRUE) {

        //skip spaces
        if ((_is_eof_reached = _skipSpacesInConfig())) {
            break;
        }

        tmpWord = libft::get_next_word(const_config_text.substr(_tmp_len));
        _tmp_len += tmpWord.size();

        // if not server in root = it is bad config
        try
        {
            if (tmpWord == SERVER_KW){
                ServerContext* tmp_server = new ServerContext();
                _servers.push_back(tmp_server);
                parseInsideServerContext(tmp_server);
            } else {
                if (tmpWord.size())
                    _badConfigError( "WORD " + tmpWord + " NOT EXPECTED THERE IN 'server' CONTEXT LEVEL");
                else
                    _badConfigError( "SYMBOL '" + std::string(1, const_config_text[_tmp_len]) + \
                                                                    "' NOT EXPECTED THERE IN 'server' CONTEXT LEVEL");
            }
        }
        catch (Config::BadConfigException & e)
        {
            exit(EXIT_FAILURE);
        }
    }
}


void Config::_badConfigError(const std::string & error_text) const {
    std::cerr << error_text << std::endl;
    throw Config::BadConfigException();
}


void Config::parseInsideServerContext(ServerContext* current_server) {
    const std::string& const_config_text = _getConfigText();

    if ((_is_eof_reached = _skipSpacesInConfig()) || \
                (const_config_text[_tmp_len] != '{') ) {
        _badConfigError("'{' AFTER 'server' KEYWORD IS NOT FOUND");
    }
    _tmp_len++; //skip found '{' symbol

    std::string tmp_word;
    std::list<std::string>::const_iterator tmp_it;


    while (TRUE) {
        //skip spaces and check EOF
        if ((_is_eof_reached = _skipSpacesInConfig())) {
            break;
        }

        //CHECK SERVER CONTEXT IS FINISHED
        if (const_config_text[_tmp_len] == '}') {
            break;
        }

        //skip spaces and check EOF
        if ((_is_eof_reached = _skipSpacesInConfig())) {
            break;
        }

        //get next word
        tmp_word = libft::get_next_word(const_config_text.substr(_tmp_len));
        _tmp_len += tmp_word.size();

        tmp_it = std::find(_serverContext.begin(), _serverContext.end(), tmp_word);

        if (tmp_it == _ite_server) { // word is not keyword
            if (tmp_word.size())
                _badConfigError( "WORD " + tmp_word + " NOT EXPECTED THERE IN 'server' CONTEXT LEVEL");
            else
                _badConfigError( "SYMBOL '" + std::string(1, const_config_text[_tmp_len]) + \
                                                                    "' NOT EXPECTED THERE IN 'server' CONTEXT LEVEL");
        }

        if (tmp_word == LOCATION_KW) {
            parseInsideLocationContext(current_server);
        } else {
            std::list<std::string> tmp_params;
            if (_isMultipleParamDirective[tmp_word]) {
                tmp_params = parseMultipleParamDirective(tmp_word);

            } else {
                tmp_params = parseSingleParamDirective(tmp_word);
            }
            _checkAndSetParams(current_server, tmp_word, tmp_params);
        }
    }
    if ((const_config_text[_tmp_len] != '}')) {
        _badConfigError("SYMBOL '}' THAT SHOULD CLOSE 'server' CONTEXT KEYWORD IS NOT FOUND");
    }
    _tmp_len++;
}

void Config::parseInsideLocationContext(ServerContext* current_server) {
    const std::string& const_config_text = _getConfigText();
    std::string tmp_word;
    std::string location_uri;
    std::list<std::string>::const_iterator tmp_it;


    if ((_is_eof_reached = _skipSpacesInConfig())) {
        _badConfigError("AFTER 'location' KEYWORD EXPECTED LOCATION URI FOR ROUTING");
    }

    location_uri = libft::get_next_word(const_config_text.substr(_tmp_len));
    _tmp_len += location_uri.size();
    _locationUriChecks(location_uri); //THROW EXCEPTION IF ERROR OCCURRED

    if ((_is_eof_reached = _skipSpacesInConfig()) || \
                (const_config_text[_tmp_len] != '{') ) {
        _badConfigError("SYMBOL '{' AFTER LOCATION URI IS EXPECTED");
    }
    _tmp_len++; //skip found '{' symbol

    LocationContext* current_location = current_server->addLocation(location_uri);

    while (TRUE) {
        //skip spaces and check EOF
        if ((_is_eof_reached = _skipSpacesInConfig())) {
            break;
        }

        //CHECK LOCATION CONTEXT IS FINISHED
        if (const_config_text[_tmp_len] == '}') {
            break;
        }

        //skip spaces and check EOF
        if ((_is_eof_reached = _skipSpacesInConfig())) {
            break;
        }

        //get next word
        tmp_word = libft::get_next_word(const_config_text.substr(_tmp_len));
        _tmp_len += tmp_word.size();

        tmp_it = std::find(_locationContext.begin(), _locationContext.end(), tmp_word);

        if (tmp_it == _ite_location) { // word is not keyword
            if (tmp_word.size())
                _badConfigError( "WORD " + tmp_word + " NOT EXPECTED THERE IN 'location' CONTEXT LEVEL");
            else
                _badConfigError( "SYMBOL '" + std::string(1, const_config_text[_tmp_len]) + \
                                                                    "' NOT EXPECTED THERE IN 'location' CONTEXT LEVEL");
        }

        std::list<std::string> tmp_params;
        if (_isMultipleParamDirective[tmp_word]) {
            tmp_params = parseMultipleParamDirective(tmp_word);
        } else {
            tmp_params = parseSingleParamDirective(tmp_word);
        }
        _checkAndSetParams(current_location, tmp_word, tmp_params);
    }
    if ((const_config_text[_tmp_len] != '}')) {
        _badConfigError("SYMBOL '}' THAT SHOULD CLOSE 'location' CONTEXT KEYWORD IS NOT FOUND");
    }
    _tmp_len++;

}


std::list<std::string> Config::parseMultipleParamDirective(const std::string &keyword) {
    const std::string& const_config_text = _getConfigText();

    std::list<std::string> params;

    std::string tmpWord;

    while(TRUE) {
        //CHECK PARAM SETTING IS FINISHED
        if (const_config_text[_tmp_len] == ';') {
            break;
        }

        //skip spaces and check EOF
        if ((_is_eof_reached = _skipSpacesInConfig())) {
            _badConfigError("EOF INSIDE CONTEXT WHEN PARSE DIRECTIVE: " + keyword );
        }
        //get next word
        tmpWord = libft::get_next_word(const_config_text.substr(_tmp_len));
        _tmp_len += tmpWord.size();
        params.push_back(tmpWord);
    }
    _tmp_len++;
    return params;
}

std::list<std::string> Config::parseSingleParamDirective(const std::string &keyword) {
    const std::string& const_config_text = _getConfigText();

    std::list<std::string> params;

    std::string tmpWord;

    if ((_is_eof_reached = _skipSpacesInConfig())) {
        _badConfigError("EOF INSIDE CONTEXT WHEN PARSE DIRECTIVE: " + keyword );
    }

    //get next word
    tmpWord = libft::get_next_word(const_config_text.substr(_tmp_len));
    _tmp_len += tmpWord.size();
    params.push_back(tmpWord);

    if ((_is_eof_reached = _skipSpacesInConfig())) {
        _badConfigError("EOF INSIDE CONTEXT WHEN PARSE DIRECTIVE: " + keyword );
    }

    //CHECK PARAM SETTING IS FINISHED
    if (const_config_text[_tmp_len] != ';') {
        _badConfigError("INVALID NUMBER OF ARGUMETNS IN " + keyword  + " DIRECTIVE");
    }
    _tmp_len++;
    return params;
}



void Config::_checkAndSetParams(AContext* current_context, const std::string& directive_keyword,
                        const std::list<std::string>& directive_params) {
    if (directive_keyword == LISTEN_KW) {
        Pair<std::string, int> host_and_port = _listenKeywordHandler(directive_params);
        static_cast<ServerContext*>(current_context)->addHostPort(host_and_port.first, host_and_port.second);
    }
    else if (directive_keyword == SERVER_NAME_KW) {
        ServerContext * serv = static_cast<ServerContext*>(current_context);
        if (!serv->get_status_is_server_names_were_updated()){
            serv->clear_server_names();
            serv->set_server_names_were_updated(true);
        }
        std::list<std::string> server_names = _serverNameKeywordHandler(directive_params);
        serv->addServerNames(server_names);
    }
    else if (directive_keyword == ERROR_PAGE_KW)
        _errorPageKeywordHandler(current_context, directive_params);
    else if (directive_keyword == CLIENT_MAX_BODY_SIZE_KW)
        _clientMaxBodySizeKeywordHandler(current_context, directive_params);
    else if (directive_keyword == LIMIT_EXCEPT_KW)
        _limitExceptKeywordHandler(current_context, directive_params);
    else if (directive_keyword == ALIAS_KW)
        _aliasKeywordHandler(current_context, directive_params);
    else if (directive_keyword == AUTOINDEX_KW)
        _autoindexExceptKeywordHandler(current_context, directive_params);
    else if (directive_keyword == INDEX_KW)
        _indexExceptKeywordHandler(current_context, directive_params);
    else
        _badConfigError("NOT EXPEXTED DIRECTIVE KEYWORD IS FOUND: '" + directive_keyword + "'");
}


// SIGNLE PART CONFIG CHECKS

void Config::_locationUriChecks(const std::string& location_uri) {
    if (!location_uri.size()) {
        _badConfigError("LOCATION URI AFTER 'location' KEYWORD IS INVALID");
    }
}


















const std::string Config::parseHost(const std::string& param) const {
    const std::string localhost_kw = "localhost";


    std::size_t found_pos = param.find(localhost_kw);
    if (found_pos != std::string::npos) {
        if (found_pos != 0) {
            _badConfigError("host not found in \"" + param + "\" of the \"listen\" directive");
        }
        return "localhost";
    } else {
        if (param.length() > std::string("255.255.255.255:65536").length()) {
            _badConfigError("host:port combination is not correct in \"" + param + "\" of the \"listen\" directive");
        }

        std::string tmp_param = param;
        size_t pos = 0;
        std::string octet;

        std::list<std::string> octets;

        while ((pos = tmp_param.find('.')) != std::string::npos) {
            octet = tmp_param.substr(0, pos);
            octets.push_back(octet);
            tmp_param.erase(0, pos + 1);
        }
        if ((pos=tmp_param.find(':')) != std::string::npos) {
            octet = tmp_param.substr(0, pos);
            octets.push_back(octet);
        } else {
            octet = tmp_param.substr(0);
            octets.push_back(octet);
        }

        if (octets.size() != 4) {
            _badConfigError("there is found not equal to 4 delimeted by '.' octets."
"it doesnt look like correct ip address: \"" + param + "\" (\"listen\" directive)");
        }

        std::size_t ip_len_to_return = 3; //3 dots between octets
        std::list<std::string>::iterator it = octets.begin();
        while (it != octets.end()) {
            if (!libft::is_correct_ip_octet(*it)) {
                _badConfigError("one of octet in host ip is not correct: \"" + param + "\" (\"listen\" directive)");
            }
            ip_len_to_return += (*it).size();
            ++it;
        }
        return param.substr(0, ip_len_to_return);
    }
}



int Config::parsePort(const std::string& param) const {
    size_t len = param.length();

    std::string error_log = "port is not found where it should be: \"" + param + "\" (\"listen\" directive)";

    if ( len <=0 || len > 5 ) {
        _badConfigError(error_log);
    }
    int port = libft::atoi(param.c_str());

    if (port <= 0 || port > 65535) {
        _badConfigError(error_log);
    }
    if (len != libft::unsigned_number_len(port, 10)) {
        _badConfigError(error_log);
    }
    return port;
}


const Pair<std::string, int > Config::_listenKeywordHandler(const std::list<std::string>& directive_params) {
    std::string tmp_word = *(directive_params.begin());

    std::string hosts;
    int port;

    std::string universal_error_log = "correct [host][:port] not found in \"" + tmp_word + "\" of the \"listen\" directive";


    //!!! 1 PATTERN
    if (tmp_word.find("*") != std::string::npos) {
        // it should be param like this 'listen *:8000' or it's invalid
        //!!! '*:' means LOCALHOST 127.0.0.0 — 127.255.255.254
        if ((tmp_word.find("*:") != 0) || (tmp_word.length() < 3)) {
            _badConfigError(universal_error_log);
        }
        port = parsePort(tmp_word.substr(2)); //end of string checks inside
        if (tmp_word.length() != (libft::unsigned_number_len(port, 10) + 2) ) {
            _badConfigError(universal_error_log);
        }
        hosts = "*";
    }

    //!!! 2 PATTERN
    else if (tmp_word.find(":") == std::string::npos) { //host:port ':' delimeter dont found
        if ((tmp_word.find('.') != std::string::npos) || (tmp_word.find("localhost") != std::string::npos)) {
            //if dots delimeters for ip octets arent found it should be just 'localhost' or just correct IP
            hosts = parseHost(tmp_word);
            if (hosts.size() != tmp_word.size()) {
                _badConfigError("invalid host in \"" + tmp_word + "\" of the \"listen\" directive");
            }
            port = DEFAULT_PORT;
        } else {
            // or it is should be just correct port
            port = parsePort(tmp_word);
            if (tmp_word.length() != libft::unsigned_number_len(port, 10) ) {
                _badConfigError(universal_error_log);
            }
            hosts = "*";
        }
    }


    //!!! 3 PATTERN
    else {
        // full host:port combination (like 127.0.0.1:8000 or localhost:8000)
        hosts = parseHost(tmp_word);
        if (tmp_word[hosts.length()] != ':') {
            _badConfigError(universal_error_log);
        }
        std::string tmp_word2 = tmp_word.substr(hosts.length() + 1); // pass ':'
        port = parsePort(tmp_word2);
        if (tmp_word.length() != (libft::unsigned_number_len(port, 10) + 1 + hosts.length()) ) {
            _badConfigError(universal_error_log);
        }
    }

    return Pair<std::string, int >(hosts, port);
}






bool Config::is_correct_serv_name(const std::string& serv_name) const {
    std::size_t len = serv_name.length();
    std::size_t found_pos = serv_name.find('*');

    std::string tmp_serv_name = serv_name;

    if (found_pos != std::string::npos) {
        if ((found_pos != 0) && (found_pos != (len - 1))) {
            return false;
        }
        if (len < 3) {
            return false;
        }
        if (found_pos == 0) {
            tmp_serv_name = serv_name.substr(1);
            if ((tmp_serv_name.find('*') != std::string::npos) || \
                    tmp_serv_name[0] != '.') {
                return false;
            }
        } else {
            tmp_serv_name = serv_name.substr(0, len - 1);
            if ((tmp_serv_name.find('*') != std::string::npos) || \
                    tmp_serv_name[len - 2] != '.') {
                    return false;
            }
        }
    }
    return true;
}

std::size_t Config::parse_until_quote_be_closed(const std::string& serv_name, std::size_t tmp_pos, char found_quote) const {
    std::size_t len = serv_name.length();

    std::string unexpected_eof_log = "unexpected end of file, expecting \";\" or \"}\"";

    if (tmp_pos == (len - 1)) {
        _badConfigError(unexpected_eof_log);
    }

    if (libft::isspace(serv_name[tmp_pos+1])) {
        _badConfigError(unexpected_eof_log);
    }
    tmp_pos++; //add to found char pos 1
    std::size_t found_pos = serv_name.find(found_quote, tmp_pos); //search from (found_pos + 1)
    if (found_pos == std::string::npos) {
        _badConfigError(unexpected_eof_log);
    }
    return found_pos;
}

void Config::find_first_occured_quote(const std::string& serv_name, std::size_t pos_to_start_search,
                                      int *found_pos, char *found_quote) const {

    std::size_t found_pos_single_quotes = serv_name.find('\'', pos_to_start_search);
    std::size_t found_pos_double_quotes = serv_name.find('"', pos_to_start_search);

    if ( (found_pos_single_quotes == std::string::npos) || \
            (found_pos_double_quotes == std::string::npos) ) {
        if ( (found_pos_single_quotes == std::string::npos) && \
            (found_pos_double_quotes == std::string::npos) ) {
            *found_pos = -1;
            *found_quote = 0; //quotes are not found
        }
        else if (found_pos_single_quotes == std::string::npos) {
            *found_quote = '"';
            *found_pos = found_pos_double_quotes;
        }
        else {
            *found_quote = '\'';
            *found_pos = found_pos_single_quotes;
        }
    }
    else if (found_pos_single_quotes < found_pos_double_quotes) {
        *found_quote = '\'';
        *found_pos = found_pos_single_quotes;
    }
    else {
        *found_quote = '"';
        *found_pos = found_pos_double_quotes;
    }
}


std::string Config::checkAndRemoveQuotes(const std::string& serv_name) const {
    std::size_t len = serv_name.length();

    char found_quote;
    int found_pos;

    find_first_occured_quote(serv_name, 0, &found_pos, &found_quote);
    if (found_pos == (-1)) { //if quotes are not found
        return serv_name;
    }

    std::size_t tmp_pos = found_pos;
    if (found_pos != 0) {
        while (tmp_pos < len) {
            find_first_occured_quote(serv_name, tmp_pos, &found_pos, &found_quote);
            if (found_pos == (-1)) { //if quotes are not found
                return serv_name;
            }
            tmp_pos = found_pos;
            tmp_pos = parse_until_quote_be_closed(serv_name, tmp_pos, found_quote); //if closed quote is not found throw Exception
            tmp_pos++; // pass closing quote
        }
        return serv_name; // nginx dont remove quotes inside string
    }
    else { //param starts from quote, it should be finished by this qoute and no one symbol after it is closed
        std::string unexpected_eof_log = "unexpected end of file, expecting \";\" or \"}\"";
        if (len < 2) {
            _badConfigError(unexpected_eof_log);
        }

        std::size_t found_pos_close_quote = serv_name.find(found_quote, 1);
        if (found_pos_close_quote != (len -1)) {
            _badConfigError(unexpected_eof_log);
        }
        return serv_name.substr(1, len - 2); //return string without border quotes
    }
}





std::list<std::string>  Config::_serverNameKeywordHandler(const std::list<std::string>& directive_params) {
    std::list<std::string> serv_names;
    std::list<std::string>::const_iterator it = directive_params.begin();

    std::string tmp_param;

    while (it != directive_params.end()) {
        tmp_param = checkAndRemoveQuotes(*it);

        if (is_correct_serv_name(tmp_param)) {
            serv_names.push_back(tmp_param);
        } else {
            _badConfigError("invalid server name or wildcard \"" + *it + "\"");
        }
        ++it;
    }
    return serv_names;
}

void Config::_errorPageKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params) {
    (void)current_context;
    std::cout << "error_page: ";
    std::cout << directive_params; //TODO: need to delete after test
}

void Config::_clientMaxBodySizeKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params) {
    (void)current_context;
    const std::string tmp = *(directive_params.begin());
    int size = libft::atoi(tmp.c_str());
    if (size < 0)
        _badConfigError("\"client_max_body_size\" directive invalid value");
    std::string sub_str =  tmp.substr(libft::unsigned_number_len(size, 10));
    std::cout << "client_max_body_size :"<<  "SIZE DIGITS: " << size << " | MEASURE: " << sub_str << std::endl;   //TODO: need to delete after test
}

void Config::_limitExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params) {
    (void)current_context;
    std::cout << "limit_except: ";
    std::cout << directive_params; //TODO: need to delete after test
}

void Config::_aliasKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params) {
    (void)current_context;
    std::cout << "alias: ";
    std::cout << directive_params; //TODO: need to delete after test
}

void Config::_autoindexExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params) {
    (void)current_context;
    std::cout << "autoindex: ";
    std::cout << directive_params; //TODO: need to delete after test
}

void Config::_indexExceptKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params) {
    (void)current_context;
    std::cout << "index: ";
    std::cout << directive_params; //TODO: need to delete after test
}





