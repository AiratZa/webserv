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

    _isMultipleParamDirective[LISTEN_KW] = true;
    _isMultipleParamDirective[SERVER_NAME_KW] = true;
    _isMultipleParamDirective[ERROR_PAGE_KW] = true;
    _isMultipleParamDirective[INDEX_KW] = true;
    _isMultipleParamDirective[LIMIT_EXCEPT_KW] = true;

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
    std::cout << "read finished" << std::endl; //TODO: NEED DELETE
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

    std::cout << "PARSING STARTED" << std::endl; //TODO: NEED DELETE

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
                std::cout << "SERVER" << std::endl; //TODO: need to delete after test
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
            std::cout << "LOCATION" << std::endl; //TODO: need to delete after test
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
        Pair<std::string, std::list<int> >* host_and_ports = _listenKeywordHandler(directive_params);
        current_context->setHostsAndPorts(*host_and_ports);
        delete host_and_ports;
    }
    else if (directive_keyword == SERVER_NAME_KW)
        _serverNameKeywordHandler(current_context, directive_params);
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
    std::size_t found_pos = param.find("localhost");
    if (found_pos != std::string::npos) {
        if (found_pos != 0) {
            _badConfigError("host not found in \"" + param + "\" of the \"listen\" directive");
        }
        return "127.0.0.1";
    }
    return ""; //for Werror flag
}

Pair<std::string, std::list<int> >* Config::_listenKeywordHandler(const std::list<std::string>& directive_params) {
    std::string tmp_word = *(directive_params.begin());
    std::string host;
    std::list<int> ports;

    if (tmp_word.find(":") == std::string::npos) { //host:port ':' delimeter dont found
        return NULL; //TODO
    }
    host =  parseHost(tmp_word);
    if (tmp_word[host.length()] != ':') {
        _badConfigError("host:port not found in \"" + tmp_word + "\" of the \"listen\" directive");
    }
    tmp_word = tmp_word.substr(host.length()+1);
    ports.push_back(libft::atoi(tmp_word.c_str()));

    Pair<std::string, std::list<int> >* host_ports = new Pair<std::string, std::list<int> >(host, ports);

    std::cout << "listen: ";
    std::cout << directive_params; //TODO: need to delete after test
    return host_ports;
}

void Config::_serverNameKeywordHandler(AContext* current_context, const std::list<std::string>& directive_params) {
    ServerContext* tmp_server = static_cast<ServerContext*>(current_context);
    std::list<std::string>::const_iterator it = directive_params.begin();

    while (it != directive_params.end()) {
        tmp_server->addServerName(*it);
        ++it;
    }

    std::cout << "server_name: ";
    std::cout << directive_params; //TODO: need to delete after test
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





