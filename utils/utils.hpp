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

#define TRUE 1
#define FALSE 1

namespace utils {
    void exitWithLog(void);
    void exitWithLog(const std::string& errorLog);

}

#endif //WEBSERV_UTILS_H
