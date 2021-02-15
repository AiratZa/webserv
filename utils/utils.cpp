#include "utils.hpp"

std::string			setDate(void)
{
    char			buffer[100];
    struct timeval	tv;
    struct tm		*tm;

    gettimeofday(&tv, NULL);
    tm = gmtime(&tv.tv_sec); //TODO write this forbidden function (and we have wrong time -3 hours)
    // tm_sec + tm_min*60 + tm_hour*3600 + tm_yday*86400 + (tm_year-70)*31536000 + ((tm_year-69)/4)*86400 - ((tm_year-1)/100)*86400 + ((tm_year+299)/400)*86400
    strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
    return std::string(buffer);
}

void utils::exitWithLog(void) {
    std::cout << std::strerror(errno) << std::endl;
    exit(errno);
}

void utils::exitWithLog(const std::string& errorLog) {
    std::cout << errorLog << std::endl;
    exit(EXIT_FAILURE);
}


