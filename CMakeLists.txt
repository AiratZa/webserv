cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set(UTILS_SRCS utils/utils.cpp utils/utils.hpp
        utils/get_next_line/get_next_line.cpp utils/get_next_line/get_next_line_utils.cpp
        utils/cpp_libft/stl_containers_operator_overloading.cpp
        utils/cpp_libft/get_next_word.cpp
        utils/cpp_libft/libft.hpp utils/cpp_libft/isspace.cpp
        utils/cpp_libft/unsigned_number_len.cpp utils/cpp_libft/atoi.cpp
        utils/custom_structures/Pair.hpp
        utils/cpp_libft/is_correct_ip_octet.cpp
        utils/cpp_libft/tolower.cpp
        utils/cpp_libft/isdigit.cpp
        utils/cpp_libft/strtoul_base.cpp
        utils/cpp_libft/string_to_lower.cpp
        utils/cpp_libft/ultostr_base.cpp
        utils/cpp_libft/stoll_base.cpp
        )


set(CONFIG_PARSING_SRCS ${UTILS_SRCS} config_parsing/Config.cpp
        config_parsing/AContext.hpp config_parsing/ServerContext.cpp config_parsing/LocationContext.cpp)


set (PROJECT_SRCS  Server.cpp WebServ.cpp request_handlers/Request.cpp response/Response.cpp ${CONFIG_PARSING_SRCS})

add_executable(webserv main.cpp ${PROJECT_SRCS} )


################################
# GTest
################################

add_subdirectory(google_test/)
enable_testing()
include_directories(google_test/googletest/include)

################################
# Unit Tests
################################
# Add test cpp file

set(CONFIG_TESTS_PATH "config_parsing/test_configs/")
set(CONFIG_DIRECTIVES_TESTS_PATH ${CONFIG_TESTS_PATH}directives/)
set(CONFIG_CONTEXTS_TESTS_PATH ${CONFIG_TESTS_PATH}contexts/)

# Config tests
file(GLOB TEST_CONTEXTS ${CONFIG_CONTEXTS_TESTS_PATH}test_location.cpp
        )

file(GLOB TEST_DIRECTIVES ${CONFIG_DIRECTIVES_TESTS_PATH}test_listen.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_server_name.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_error_page.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_index.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_limit_except.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_client_max_body_size.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_alias.cpp
        ${CONFIG_DIRECTIVES_TESTS_PATH}test_autoindex.cpp
        )


set (TEST_FILES ${TEST_CONTEXTS} ${TEST_DIRECTIVES} ${CONFIG_TESTS_PATH}fixtures.cpp )

set_source_files_properties(${CONFIG_PARSING_SRCS} ${TEST_FILES} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror)

add_executable( runConfigParsingUnitTests ${TEST_FILES} ${CONFIG_PARSING_SRCS})
# Link test executable against gtest & gtest_main
target_link_libraries(runConfigParsingUnitTests gtest gtest_main)
add_test( runConfigParsingUnitTests runConfigParsingUnitTests )


#===============================================================

# Engine tests

set(ENGINE_TESTS_PATH "engine_tests/")

file(GLOB TEST_ENGINE ${ENGINE_TESTS_PATH}test_servers.cpp
)


set_source_files_properties(${TEST_ENGINE} ${PROJECT_SRCS} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror)

add_executable( runEngineUnitTests ${TEST_ENGINE} ${PROJECT_SRCS})

# Link test executable against gtest & gtest_main
target_link_libraries(runEngineUnitTests gtest gtest_main)
add_test( runEngineUnitTests runEngineUnitTests )

# Requests tests

set(REQUESTS_TESTS_PATH "requests_test/")

file(GLOB TEST_REQUESTS ${REQUESTS_TESTS_PATH}test_servers.cpp
)


set_source_files_properties(${TEST_REQUESTS} ${PROJECT_SRCS} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror)

add_executable( runRequestsUnitTests ${TEST_REQUESTS} ${PROJECT_SRCS})

# Link test executable against gtest & gtest_main
target_link_libraries(runRequestsUnitTests gtest gtest_main)
add_test( runRequestsUnitTests runRequestsUnitTests )


# libft::strtoul_base() tests
set(TESTS_PATH "request_handlers/tests/")

file(GLOB TEST_FILES ${TESTS_PATH}strtoul_base_tests.cpp)

set_source_files_properties(${TEST_FILES} ${PROJECT_SRCS} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror)

add_executable( runStrtoulBaseUnitTests ${TEST_FILES} ${PROJECT_SRCS})

# Link test executable against gtest & gtest_main
target_link_libraries(runStrtoulBaseUnitTests gtest gtest_main)
add_test( runStrtoulBaseUnitTests runStrtoulBaseUnitTests )

# libft::ultostr_base() tests
set(TESTS_PATH "response/tests/")

file(GLOB TEST_FILES ${TESTS_PATH}ultostr_base_tests.cpp)

set_source_files_properties(${TEST_FILES} ${PROJECT_SRCS} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror)

add_executable( runUltostrBaseUnitTests ${TEST_FILES} ${PROJECT_SRCS})

# Link test executable against gtest & gtest_main
target_link_libraries(runUltostrBaseUnitTests gtest gtest_main)
add_test( runUltostrBaseUnitTests runUltostrBaseUnitTests )


# ===============================================================

# Utils functions tests

set(UTILS_TESTS_PATH "utils/tests")

file(GLOB TEST_STR_TO_NBR ${UTILS_TESTS_PATH}/cpp_libft/test_stoll_base.cpp
        )


set_source_files_properties(${TEST_ENGINE} ${PROJECT_SRCS} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror)

add_executable( runUtilsUnitTests ${TEST_STR_TO_NBR} ${PROJECT_SRCS})

# Link test executable against gtest & gtest_main
target_link_libraries(runUtilsUnitTests gtest gtest_main)
add_test( runUtilsUnitTests runUtilsUnitTests )