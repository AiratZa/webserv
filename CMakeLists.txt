cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv main.cpp utils/utils.cpp Server.cpp WebServ.cpp config_parsing/Config.cpp)