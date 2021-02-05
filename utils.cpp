#include "utils.h"

void utils::exitWithLog(void) {
    std::cout << std::strerror(errno) << std::cout;
    exit(EXIT_FAILURE);
}