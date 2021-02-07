cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv main.cpp utils/utils.cpp
        utils/get_next_line/get_next_line.cpp utils/get_next_line/get_next_line_utils.cpp


        Server.cpp WebServ.cpp config_parsing/Config.cpp config_parsing/AContext.hpp config_parsing/ADirective.hpp config_parsing/MainContext.hpp config_parsing/HttpContext.hpp config_parsing/ServerContext.hpp config_parsing/LocationContext.hpp utils/cpp_libft/libft.hpp utils/cpp_libft/isspace.cpp)