//
// Created by airat on 15.02.2021.
//

#include "gtest/gtest.h"
#include "../WebServ.hpp"

#include <string>

TEST(WebServEngine, servers_count_tests) {

    WebServ webserv = WebServ("../config_parsing/test_configs/2.conf");

    ASSERT_EQ(webserv.getServersCount(), 3);
}

TEST(StringTests, len) {

    std::string str = "Hello";
    ASSERT_EQ(str.length(), 4);
}