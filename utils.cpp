#include "utils.hpp"

void utils::exitWithLog(void) {
    std::cout << std::strerror(errno) << std::endl;
    exit(errno);
}