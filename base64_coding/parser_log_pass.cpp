#include <iostream>
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../get_next_line/get_next_line.hpp"

std::vector<std::string>    parser_log_pass(std::string const& file) {
    std::vector<std::string> log_pass;

    int fd_file = open(file.c_str(), O_RDONLY);
    // if (fd_file < 0) 
    //     utils::exitWithLog("Error happened when try to open auth_basic_user_file");

    char *str;
    int  read;

    while ((read = get_next_line(fd_file, &str)) == 1) {
        log_pass.push_back(str);
        delete str;
    }
    log_pass.push_back(str);
    delete str;

    // if (read != 0)
    //     utils::exitWithLog("Error happened when read auth_basic_user_file");
    close(fd_file);
    return log_pass;
}