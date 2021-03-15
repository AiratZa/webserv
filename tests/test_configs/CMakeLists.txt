cmake_minimum_required(VERSION 3.17)
project(config_parsing_tests)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(../../tests/google_test/)
include_directories(../../tests/google_test/googletest/include)

set(SOURCE_FILES
        listen/main.cpp)

file(GLOB TEST_LISTEN "listen/*.hpp" "listen/*.cpp")
#file(GLOB TEST_STACK_SRC "tests/test_stack/*.hpp" "tests/test_stack/*.cpp")
#file(GLOB TEST_QUEUE_SRC "tests/test_queue/*.hpp" "tests/test_queue/*.cpp")

set (TEST_FILES ${TEST_LISTEN} )

set_source_files_properties(${SOURCE_FILES} ${TEST_FILES} PROPERTIES COMPILE_FLAGS -Wall -Wextra -Werror -std=c++98)

add_executable(config_parsing_tests ${SOURCE_FILES} ${TEST_FILES} )
target_link_libraries( gtest )


