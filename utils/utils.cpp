#include "utils.hpp"



std::string			utils::setDate(void)
{
    char			buffer[100];
    struct timeval	tv;
    struct tm		*my_tm = new tm;
    long            tmp_sec;
    int             exep_year = 2;
    long             months[11] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

    gettimeofday(&tv, NULL);
    my_tm->tm_year = 70;
    tmp_sec = tv.tv_sec + 10800; //смещение часового пояса на +3 часа
    my_tm->tm_wday = (tmp_sec / 86400 % 7 + 4) % 7;
    while ((tmp_sec > 31536000 && (exep_year % 4 != 0)) ||
            (tmp_sec > 31622400 && (exep_year % 4 == 0))) {
        if (exep_year % 4 == 0)
            tmp_sec -= 31622400;
        else
            tmp_sec -= 31536000;
        ++my_tm->tm_year;
        ++exep_year;
    }
    long            tmp_mon_sec = tmp_sec;
    int i = 0;
    if (exep_year % 4 == 0)
            ++months[1];
    my_tm->tm_mon = 0;
    while (tmp_mon_sec > (months[i] * 86400)) {
        ++my_tm->tm_mon;
        tmp_mon_sec -= months[i] * 86400;
        ++i;
    }
    my_tm->tm_mday = tmp_mon_sec / 86400 + 1;
    my_tm->tm_yday = tmp_sec / 86400 ;
    tmp_sec %= 86400;
    my_tm->tm_hour = tmp_sec / 3600;
    tmp_sec %= 3600;
    my_tm->tm_min = tmp_sec / 60;
    tmp_sec %= 60;
    my_tm->tm_sec = tmp_sec;
    strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", my_tm);
    return std::string(buffer);
}

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


