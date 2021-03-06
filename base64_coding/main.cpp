#include "base64.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../get_next_line/get_next_line.hpp"

std::vector<std::string>    parser_log_pass(std::string file) {
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

bool    find_log_pass(std::vector<std::string> log_pass, std::string const& reqests_log_pass) {
    std::vector<std::string>::iterator it = log_pass.begin();
    std::string decoded_reqest_log_pass = base64_decode(reqests_log_pass);

    while (it != log_pass.end()) {
        if (*it == decoded_reqest_log_pass)
            return true;
    }
    return false;
}


int main() {
    std::vector<std::string> log_pass = parser_log_pass("htpasswd");
    std::cout << find_log_pass(log_pass, "YWRtaW46YWRtaW4=") << std::endl;
    return 0;
}