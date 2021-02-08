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
                parseInsideServerContext();
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


void Config::parseInsideServerContext(void) {
    const std::string& const_config_text = _getConfigText();

    if ((_is_eof_reached = _skipSpacesInConfig()) || \
                (const_config_text[_tmp_len] != '{') ) {
        _badConfigError("'{' AFTER SERVER KEYWORD IS NOT FOUND");
    }
    _tmp_len++; //skip found '{' symbol

    std::string tmpWord;
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
        tmpWord = libft::get_next_word(const_config_text.substr(_tmp_len));
        _tmp_len += tmpWord.size();

        tmp_it = std::find(_serverContext.begin(), _serverContext.end(), tmpWord);

        if (tmp_it == _ite_server) { // word is not keyword
            if (tmpWord.size())
                _badConfigError( "WORD " + tmpWord + " NOT EXPECTED THERE IN 'server' CONTEXT LEVEL");
            else
                _badConfigError( "SYMBOL '" + std::string(1, const_config_text[_tmp_len]) + \
                                                                    "' NOT EXPECTED THERE IN 'server' CONTEXT LEVEL");
        }

        if (tmpWord == LOCATION_KW) {
            std::cout << "LOCATION" << std::endl; //TODO: need to delete after test

            parseInsideLocationContext();
        } else if (_isMultipleParamDirective[tmpWord]) {
            std::list<std::string> tmp = parseMultipleParamDirective(tmpWord);
            std::cout << tmpWord <<" ==> "  << tmp << std::endl; //TODO: need to delete after test

        } else {
            std::list<std::string> tmp = parseSingleParamDirective(tmpWord);
            std::cout << tmpWord <<" ==> "  << tmp << std::endl; //TODO: need to delete after test
        }
    }
    if ((const_config_text[_tmp_len] != '}')) {
        _badConfigError("SYMBOL '}' THAT SHOULD CLOSE 'server' CONTEXT KEYWORD IS NOT FOUND");
    }
    _tmp_len++;
}

void Config::parseInsideLocationContext(void) {

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
        _badConfigError("DIRECTIVE: " + keyword  + " IS NOT FINISHED WITH ';' CHAR");
    }
    _tmp_len++;
    return params;
}




