//
// Created by airat on 06.02.2021.
//

#include "Config.hpp"
#include "../utils/get_next_line/get_next_line.hpp"

Config::Config(const std::string &path_to_config) {
    fillConfigTextFromFile(path_to_config);
    splitConfigTextIntoBlocks(_configText);
}

void Config::fillConfigTextFromFile(const std::string &path_to_config) {
    int file = open(path_to_config.c_str(), O_RDONLY);
    if (file < 0)
        utils::exitWithLog("Error happened when try to open config file :(");

    char *str;
    int ret;

    while ((ret = get_next_line(file, &str)) == 1) {
        _configText += str;
        _configText += "\n";
        delete str;
    }

    if (ret != 0) {
        utils::exitWithLog("Error happened when read config file :(");
    }
    close(file);
}

void Config::splitConfigTextIntoBlocks(const std::string& constConfigText) {


    std::cout << constConfigText << std::endl;

}

//std::string replaceIfOccurs (std::string& line, const std::string& s1, const std::string& s2) {
//    size_t start_pos;
//    size_t tmp_pos = 0;
//
//    while ((start_pos = line.find(s1, tmp_pos)) != std::string::npos) {
//        line.replace(start_pos, s1.length(), s2);
//        tmp_pos = start_pos + s2.length();
//    }
//    return (line);
//}


