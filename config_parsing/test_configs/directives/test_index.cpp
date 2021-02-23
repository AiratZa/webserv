//
// Created by airat on 20.02.2021.
//


#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../fixtures.hpp"

//NEGATIVE SCENARIOS INDEX

TEST(IndexDirectiveTests, negative_scenarios_with_config_context_1)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "location =/ {"
                            "index ;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}


//POSITIVE SCENARIOS LOCATION CONTEXT

TEST(IndexDirectiveTests, positive_scenarios_inheritance_wtih_context)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "location =/abc {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    EXPECT_EQ(*((*it)->getIndexPagesDirectiveInfo().begin()), "index.html");

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    std::list<std::string> loc_level_indexes = (*locations.begin())->getIndexPagesDirectiveInfo();

    std::list<std::string>::const_iterator it_loc = loc_level_indexes.begin();

    EXPECT_EQ(*it_loc, "index.html");
}




TEST(IndexDirectiveTests, positive_scenarios_redifinition_wtih_context)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "location =/abc {"
                            "index main.php index1.html;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    EXPECT_EQ(*((*it)->getIndexPagesDirectiveInfo().begin()), "index.html");

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    std::list<std::string> loc_level_indexes = (*locations.begin())->getIndexPagesDirectiveInfo();

    std::list<std::string>::const_iterator it_loc = loc_level_indexes.begin();

    EXPECT_EQ(*it_loc, "main.php");
    ++it_loc;

    EXPECT_EQ(*it_loc, "index1.html");
}

TEST(IndexDirectiveTests, positive_scenarios_default_values)
{
    std::string conf_text = "server {"
                            "location =/abc {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    EXPECT_EQ((*it)->getIndexPagesDirectiveInfo().size(), 2);
    EXPECT_EQ(((*it)->getIndexPagesDirectiveInfo().front()), "index");
    EXPECT_EQ(((*it)->getIndexPagesDirectiveInfo().back()), "index.html");

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    std::list<std::string> loc_level_indexes = (*locations.begin())->getIndexPagesDirectiveInfo();

    EXPECT_EQ(loc_level_indexes.size(), 2);
    EXPECT_EQ(loc_level_indexes.front(), "index");
    EXPECT_EQ(loc_level_indexes.back(), "index.html");
}

