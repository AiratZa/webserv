//
// Created by airat on 04.02.2021.
//

#ifndef WEBSERV_UTILS_H
#define WEBSERV_UTILS_H

#include <iostream>
#include <cerrno> //errno
#include <cstring> //for strerror
#include <cstdlib>
#include <list>
#include <map>
#include <sys/time.h>


#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD "\x1B[1m"
#define UNDL "\x1B[4m"

namespace utils {
    void exitWithLog(void);
    void exitWithLog(const std::string& errorLog);
    std::string			setDate(void);

    bool isHttpMethod(const std::string& value);

    template <class Container, class Value>
    bool is_exists_in_container(const Container& container_to_search_in, const Value& value_to_search) {
        if (container_to_search_in.find(value_to_search) == container_to_search_in.end()) {
            return false;
        }
        return true;
    }
}

#endif //WEBSERV_UTILS_H
