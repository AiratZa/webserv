#include "utils.hpp"

void utils::exitWithLog(void) {
    std::cout << std::strerror(errno) << std::endl;
    exit(errno);
}

void utils::exitWithLog(const std::string& errorLog) {
    std::cout << errorLog << std::endl;
    exit(EXIT_FAILURE);
}


