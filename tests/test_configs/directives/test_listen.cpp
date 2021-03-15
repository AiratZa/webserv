//
// Created by airat on 10.02.2021.
//

#include "gtest/gtest.h"
#include "../../../config_parsing/Config.hpp"

//NEGATIVE SCENARIOS HOST

TEST(ListenDirectiveHostTests, localhost_not_from_start)
{
    Config tmp;
    std::string host = "1localhost:8000";
    EXPECT_THROW(tmp.parseHost(host), Config::BadConfigException );
}


TEST(ListenDirectiveHostTests, ip_with_negative_number)
{
    Config tmp;
    std::string host = "127.0.0.-1:8000";
    EXPECT_THROW(tmp.parseHost(host), Config::BadConfigException );

    std::string host2 = "127.0.-1.0:8000";
    EXPECT_THROW(tmp.parseHost(host2), Config::BadConfigException );
}

TEST(ListenDirectiveHostTests, ip_with_max_octet_plus_1)
{
    Config tmp;
    std::string host = "127.0.0.256:8000";
    EXPECT_THROW(tmp.parseHost(host), Config::BadConfigException );

    std::string host2 = "2147483647123.0.0.255:8000";
    EXPECT_THROW(tmp.parseHost(host2), Config::BadConfigException );
}

TEST(ListenDirectiveHostTests, ip_with_more_4_octet)
{
    Config tmp;
    std::string host = "127.0.0.1.1:8000";
    EXPECT_THROW(tmp.parseHost(host), Config::BadConfigException );

    std::string host2 = "127.0.0.1.255:8000";
    EXPECT_THROW(tmp.parseHost(host2), Config::BadConfigException );
}

TEST(ListenDirectiveHostTests, ip_starts_not_from_digit)
{
    Config tmp;
    std::string host = "127. 0.0.1:8000";
    EXPECT_THROW(tmp.parseHost(host), Config::BadConfigException );
}

//POSITIVE SCENARIOS HOST

TEST(ListenDirectiveHostTests, localhost)
{
    Config tmp;
    std::string host = "localhost:8000";
    EXPECT_EQ(tmp.parseHost(host), "localhost" );
}

TEST(ListenDirectiveHostTests, min_ip)
{
    Config tmp;
    std::string host = "0.0.0.0:8000";
    EXPECT_EQ(tmp.parseHost(host), "0.0.0.0" );
}

TEST(ListenDirectiveHostTests, max_ip)
{
    Config tmp;
    std::string host = "255.255.255.255:8000";
    EXPECT_EQ(tmp.parseHost(host), "255.255.255.255" );
}

//!!!=============================

//NEGATIVE SCENARIOS PORT

TEST(ListenDirectivePortsTests, port_min_minus_1)
{
    Config tmp;
    std::string port = "0";
    EXPECT_THROW(tmp.parsePort(port), Config::BadConfigException );
}

TEST(ListenDirectivePortsTests, port_negative)
{
    Config tmp;
    std::string port = "-1";
    EXPECT_THROW(tmp.parsePort(port), Config::BadConfigException );
}


TEST(ListenDirectivePortsTests, too_long)
{
    Config tmp;
    std::string port = "-98516541654165465165165165161651651";
    EXPECT_THROW(tmp.parsePort(port), Config::BadConfigException );
}

TEST(ListenDirectivePortsTests, empty_str)
{
    Config tmp;
    std::string port = "";
    EXPECT_THROW(tmp.parsePort(port), Config::BadConfigException );
}

TEST(ListenDirectivePortsTests, max_plus_1)
{
    Config tmp;
    std::string port = "65536";
    EXPECT_THROW(tmp.parsePort(port), Config::BadConfigException );
}

TEST(ListenDirectivePortsTests, with_unexpected_chars_start)
{
    Config tmp;
    std::string port = "/8080";
    EXPECT_THROW(tmp.parsePort(port), Config::BadConfigException );
}

TEST(ListenDirectivePortsTests, with_unexpected_chars_end)
{
    Config tmp;
    std::string port = "8080!";
    EXPECT_THROW(tmp.parsePort(port), Config::BadConfigException );
}

//POSITIVE SCENARIOS PORT


TEST(ListenDirectivePortsTests, min)
{
    Config tmp;
    std::string port = "1";
    EXPECT_EQ(tmp.parsePort(port), 1);
}

TEST(ListenDirectivePortsTests, max)
{
    Config tmp;
    std::string port = "65535";
    EXPECT_EQ(tmp.parsePort(port), 65535);
}


//!!!=============================

//NEGATIVE SCENARIOS PORT

TEST(ListenDirectiveHOST_PORTTests, asterisk_plus_port_neg)
{
    Config tmp;
    std::list<std::string> tmp_list;

    tmp_list.push_back("*: 8000");
    tmp_list.push_back("* :8000");
    tmp_list.push_back(" *:8000");
    tmp_list.push_back("*:80000");
    tmp_list.push_back("*:65536");
    tmp_list.push_back("*:-1");
    tmp_list.push_back("* :1");
    tmp_list.push_back("*:1a");

    std::list<std::string>::iterator it = tmp_list.begin();

    while (it != tmp_list.end()) {

        std::list<std::string> param_list;
        param_list.push_back(*it);

        EXPECT_THROW(tmp._listenKeywordHandler(param_list), Config::BadConfigException );
        ++it;
    }
}

TEST(ListenDirectiveHOST_PORTTests, localhost_or_ip_wtihout_port_neg)
{
    Config tmp;
    std::list<std::string> tmp_list;

    tmp_list.push_back(" localhost");
    tmp_list.push_back("1localhost");
    tmp_list.push_back("localhost_");
    tmp_list.push_back("locaLhost");
    tmp_list.push_back(" 255.255.255.255");
    tmp_list.push_back("a255.255.255.255");
    tmp_list.push_back("255.255e.255.254");
    tmp_list.push_back("0.0.0.1 ");
    tmp_list.push_back("0.0.0.1b");

    std::list<std::string>::iterator it = tmp_list.begin();

    while (it != tmp_list.end()) {

        std::list<std::string> param_list;
        param_list.push_back(*it);

        EXPECT_THROW(tmp._listenKeywordHandler(param_list), Config::BadConfigException );
        ++it;
    }
}

TEST(ListenDirectiveHOST_PORTTests, just_port_neg)
{
    Config tmp;
    std::list<std::string> tmp_list;

    tmp_list.push_back("-1");
    tmp_list.push_back("0");
    tmp_list.push_back(" 1");
    tmp_list.push_back("a1");
    tmp_list.push_back("1q");
    tmp_list.push_back("65536");
    tmp_list.push_back("9999999999999999999999999");
    tmp_list.push_back("1234a");
    tmp_list.push_back("456 ");
    tmp_list.push_back("_");
    tmp_list.push_back("");

    std::list<std::string>::iterator it = tmp_list.begin();

    while (it != tmp_list.end()) {

        std::list<std::string> param_list;
        param_list.push_back(*it);

        EXPECT_THROW(tmp._listenKeywordHandler(param_list), Config::BadConfigException );
        ++it;
    }
}

TEST(ListenDirectiveHOST_PORTTests, host_port_combo_neg)
{
    Config tmp;
    std::list<std::string> tmp_list;

    tmp_list.push_back("127.0.0.18000");
    tmp_list.push_back("127.0.0.1..:8000");
    tmp_list.push_back("127.0.0.1_8000");
    tmp_list.push_back("127.0.0.1: 8000");
    tmp_list.push_back("127.0.0.1: -1");
    tmp_list.push_back("localhost:/8080");
    tmp_list.push_back("localhost: 8080");
    tmp_list.push_back("localhost: 1");
    tmp_list.push_back("loqalhost:65535");
    tmp_list.push_back("localhost:");
    tmp_list.push_back("localhost:65536");
    tmp_list.push_back("localhost:-1");
    tmp_list.push_back(":");

    std::list<std::string>::iterator it = tmp_list.begin();

    while (it != tmp_list.end()) {

        std::list<std::string> param_list;
        param_list.push_back(*it);

        EXPECT_THROW(tmp._listenKeywordHandler(param_list), Config::BadConfigException );
        ++it;
    }
}

//POSITIVE SCENARIOS PORT


TEST(ListenDirectiveHOST_PORTTests, asterisk_plus_port_pos)
{
    Config tmp;

    {
        std::list<std::string> param_list;
        param_list.push_back("*:8000");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "*");
        EXPECT_EQ(val.second, 8000);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("*:1");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "*");
        EXPECT_EQ(val.second, 1);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("*:9876");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "*");
        EXPECT_EQ(val.second, 9876);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("*:65535");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "*");
        EXPECT_EQ(val.second, 65535);
    }
}

TEST(ListenDirectiveHOST_PORTTests, localhost_or_ip_wtihout_port_pos)
{
    Config tmp;

    {
        std::list<std::string> param_list;
        param_list.push_back("localhost");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "127.0.0.1");
        EXPECT_EQ(val.second, DEFAULT_PORT);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("127.0.0.1");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "127.0.0.1");
        EXPECT_EQ(val.second, DEFAULT_PORT);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("0.0.0.0");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "0.0.0.0");
        EXPECT_EQ(val.second, DEFAULT_PORT);
    }
}

TEST(ListenDirectiveHOST_PORTTests, just_port_pos)
{
    Config tmp;

    {
        std::list<std::string> param_list;
        param_list.push_back("1");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "*");
        EXPECT_EQ(val.second, 1);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("65535");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "*");
        EXPECT_EQ(val.second, 65535);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("8095");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "*");
        EXPECT_EQ(val.second, 8095);
    }
}

TEST(ListenDirectiveHOST_PORTTests, host_port_combo_pos)
{
    Config tmp;

    {
        std::list<std::string> param_list;
        param_list.push_back("0.0.0.0:80");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "0.0.0.0");
        EXPECT_EQ(val.second, 80);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("127.0.0.2:65535");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "127.0.0.2");
        EXPECT_EQ(val.second, 65535);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("255.255.255.255:1");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "255.255.255.255");
        EXPECT_EQ(val.second, 1);
    }
    // localhosts
    {
        std::list<std::string> param_list;
        param_list.push_back("localhost:80");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "127.0.0.1");
        EXPECT_EQ(val.second, 80);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("localhost:65535");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "127.0.0.1");
        EXPECT_EQ(val.second, 65535);
    }
    {
        std::list<std::string> param_list;
        param_list.push_back("localhost:1");
        const Pair<std::string, int>& val = tmp._listenKeywordHandler(param_list);
        EXPECT_EQ(val.first, "127.0.0.1");
        EXPECT_EQ(val.second, 1);
    }
}


