//
// Created by airat on 06.02.2021.
//

#include "Config.hpp"

Config::Config(const std::string &path_to_config) {
    int file = open(path_to_config.c_str(), O_RDONLY);
    if (file < 0)
        utils::exitWithLog();
    std::cout << "ФАйл успешно открылся" << std::endl;
}
