#include "utils.hpp"

bool utils::isHttpMethod(const std::string& value) {
    std::string HTTP_METHODS[8] = {"GET",
                                    "HEAD",
                                    "POST",
                                    "PUT",
                                    "DELETE",
                                    "CONNECT",
                                    "OPTIONS",
                                    "TRACE",
    };
    for (int i = 0; i < 8; i++) {
        if (HTTP_METHODS[i] == value) {
            return true;
        }
    }
    return false;
}


void utils::exitWithLog(void) {
    std::cout << std::strerror(errno) << std::endl;
    exit(errno);
}

void utils::exitWithLog(const std::string& errorLog) {
    std::cout << errorLog << std::endl;
    exit(EXIT_FAILURE);
}


