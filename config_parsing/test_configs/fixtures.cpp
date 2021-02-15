//
// Created by airat on 14.02.2021.
//

#include <iostream>
#include <exception>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>

#include "fixtures.hpp"

void createTestConfigFromString(const std::string& file_name,
                                      const std::string& config_text) {
    int conf_file = open(file_name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);

    if (conf_file <= 0) {
        std::cerr << std::strerror(errno) << std::endl;
        throw std::exception();
    }
    write(conf_file, config_text.c_str(), config_text.length());
    close(conf_file);
}