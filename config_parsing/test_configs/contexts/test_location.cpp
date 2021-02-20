//
// Created by airat on 18.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../../ServerContext.hpp"
#include "../fixtures.hpp"



TEST(LocationContextTests, innvalid_location_modifier_neg)
{
    Config tmp;

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("a");
        location_uri_params.push_back("/");
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("==");
        location_uri_params.push_back("/");
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back(";");
        location_uri_params.push_back("/");
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }
    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("{");
        location_uri_params.push_back("/");
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }
    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("}");
        location_uri_params.push_back("/");
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }
    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("/");
        location_uri_params.push_back("/");
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }
}

TEST(LocationContextTests, invalid_number_of_arguments_neg)
{
    Config tmp;

    {
        std::list<std::string> location_uri_params;
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("=");
        location_uri_params.push_back("/");
        location_uri_params.push_back("/");
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }

    {
        std::list<std::string> location_uri_params;
        for (int i = 0; i < 2000000000; i++) {
            location_uri_params.push_back("/");
        }
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }
}


//POSITIVE SCENARIOS PORT


//TEST(ListenDirectivePortsTests, min)
//{
//    Config tmp;
//    std::string port = "1";
//    EXPECT_EQ(tmp.parsePort(port), 1);
//}