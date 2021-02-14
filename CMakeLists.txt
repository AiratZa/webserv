cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(UTILS_SRCS utils/utils.cpp
        utils/get_next_line/get_next_line.cpp utils/get_next_line/get_next_line_utils.cpp
        utils/cpp_libft/stl_containers_operator_overloading.cpp
        utils/cpp_libft/get_next_word.cpp
        utils/cpp_libft/libft.hpp utils/cpp_libft/isspace.cpp
        utils/cpp_libft/unsigned_number_len.cpp utils/cpp_libft/atoi.cpp
        utils/custom_structures/Pair.hpp
        utils/cpp_libft/is_correct_ip_octet.cpp
        utils/cpp_libft/tolower.cpp)


set(CONFIG_PARSING_SRCS ${UTILS_SRCS} config_parsing/Config.cpp
        config_parsing/AContext.hpp config_parsing/ServerContext.cpp config_parsing/LocationContext.cpp)


add_executable(webserv main.cpp
        Server.cpp WebServ.cpp request_handlers/Request.cpp response/Response.cpp ${CONFIG_PARSING_SRCS} )


################################
# GTest
################################

add_subdirectory(google_test/)
#ADD_SUBDIRECTORY (gtest-1.6.0)
enable_testing()
include_directories(google_test/googletest/include)
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file

set(CONFIG_TESTS_PATH "config_parsing/test_configs/")
set(CONFIG_DIRECTIVES_TESTS_PATH ${CONFIG_TESTS_PATH}directives/)

file(GLOB TEST_DIRECTIVES ${CONFIG_DIRECTIVES_TESTS_PATH}test_listen.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_server_name.cpp)
#file(GLOB TEST_STACK_SRC "tests/test_stack/*.hpp" "tests/test_stack/*.cpp")
#file(GLOB TEST_QUEUE_SRC "tests/test_queue/*.hpp" "tests/test_queue/*.cpp")

set (TEST_FILES ${TEST_DIRECTIVES} )

set_source_files_properties(${CONFIG_PARSING_SRCS} ${TEST_FILES} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror)

add_executable( runConfigParsingUnitTests ${TEST_FILES} ${CONFIG_PARSING_SRCS})
# Link test executable against gtest & gtest_main
target_link_libraries(runConfigParsingUnitTests gtest gtest_main)
add_test( runConfigParsingUnitTests runConfigParsingUnitTests )