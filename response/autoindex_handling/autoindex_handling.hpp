//
// Created by airat on 22.02.2021.
//

#ifndef WEBSERV_AUTOINDEX_HANDLING_HPP
#define WEBSERV_AUTOINDEX_HANDLING_HPP

#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>

// readdir
#include <sys/types.h>
#include <dirent.h>

// stat
#include <sys/stat.h>

// strftime ...
#include <time.h>

// gettimeofday
#include <sys/time.h>

#include "../../utils/cpp_libft/libft.hpp"
#include "../../WebServ.hpp"

void write_html();
void dir_opers();

#endif //WEBSERV_AUTOINDEX_HANDLING_HPP
