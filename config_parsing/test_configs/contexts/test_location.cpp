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
        for (int i = 0; i < 1000; i++) {
            location_uri_params.push_back("/");
        }
        EXPECT_THROW(tmp._locationKeywordHandler(location_uri_params), Config::BadConfigException );
    }
}

TEST(LocationContextTests, negative_scenarios_with_config_context_1)
{
    std::string conf_text = "server {"
                            "location =/abc {}"
                            "location /abc {}"
                            "location /xyz {}"
                            "location /xyz {}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(LocationContextTests, negative_scenarios_with_config_context_2)
{
    std::string conf_text = "server {"
                            "location =/abc {}"
                            "location =/abc {}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

//POSITIVE SCENARIOS LOCATION CONTEXT


TEST(LocationContextTests, positive_scenarios)
{
    Config tmp;

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("/");
        std::list<std::string> returned_val = tmp._locationKeywordHandler(location_uri_params);

        EXPECT_EQ(returned_val, location_uri_params);
    }

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("/abc");
        std::list<std::string> returned_val = tmp._locationKeywordHandler(location_uri_params);

        EXPECT_EQ(returned_val, location_uri_params);
    }

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("=/");
        std::list<std::string> returned_val = tmp._locationKeywordHandler(location_uri_params);

        location_uri_params.clear();
        location_uri_params.push_back("=");
        location_uri_params.push_back("/");
        EXPECT_EQ(returned_val, location_uri_params);
    }

    {
        std::list<std::string> location_uri_params;
        location_uri_params.push_back("=/abc");
        std::list<std::string> returned_val = tmp._locationKeywordHandler(location_uri_params);

        location_uri_params.clear();
        location_uri_params.push_back("=");
        location_uri_params.push_back("/abc");
        EXPECT_EQ(returned_val, location_uri_params);
    }
}


TEST(LocationContextTests, positive_scenarios_with_config_context_1)
{
    std::string conf_text = "server {"
                            "location =/ {}"
                            "location / {}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 2);

    std::list<LocationContext*>::const_iterator it_l = locations.begin();
    EXPECT_EQ((*it_l)->getLocationPath(), "= /");

    ++it_l;
    EXPECT_EQ((*it_l)->getLocationPath(), "/");
}


TEST(LocationContextTests, positive_scenarios_with_config_context_2)
{
    std::string conf_text = "server {"
                            "location =/abc {}"
                            "location /abc {}"
                            "location /xyz {}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 3);

    std::list<LocationContext*>::const_iterator it_l = locations.begin();
    EXPECT_EQ((*it_l)->getLocationPath(), "= /abc");

    ++it_l;
    EXPECT_EQ((*it_l)->getLocationPath(), "/abc");

    ++it_l;
    EXPECT_EQ((*it_l)->getLocationPath(), "/xyz");

}













