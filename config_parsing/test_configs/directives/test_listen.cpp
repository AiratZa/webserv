//
// Created by airat on 10.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"

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

//TEST(ListenDirectivePortsTests, localhost_not_from_start)
//{
//    Config tmp;
//    std::string host = "1localhost:8000";
//    EXPECT_THROW(tmp.parseHost(host), Config::BadConfigException );
//}
