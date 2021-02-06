//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../utils/utils.hpp"

class Config {
public:
    Config() { }
    Config(const std::string &path_to_config);

private:


};

#endif //WEBSERV_CONFIG_HPP
