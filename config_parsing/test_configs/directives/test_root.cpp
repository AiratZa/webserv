//
// Created by airat on 21.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../fixtures.hpp"

//NEGATIVE SCENARIOS ROOT

TEST(RootDirectiveTests, negtive_with_context_without_params)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "root ;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}


TEST(RootDirectiveTests, negtive_with_context_second_declaration)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "root /1;"
                            "root /2;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(RootDirectiveTests, negtive_with_context_2_more_params)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "root /1 /2;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

////POSITIVE SCENARIOS ROOT

TEST(RootDirectiveTests, positive_scenarios_wtih_context)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "root \"/home\";"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    std::string root = (*locations.begin())->getRootPath();

    EXPECT_EQ(root, "/home");
}

TEST(RootDirectiveTests, positive_scenarios_wtih_context_default_value)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    EXPECT_EQ((*it)->getRootPath(), "html");

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    EXPECT_EQ((locations.front())->getRootPath(), "html");
}


TEST(RootDirectiveTests, positive_scenarios_wtih_context_inheritance)
{
    std::string conf_text = "server {"
                            "root sweetHome;"
                            "location =/ {"
                            "index index.html;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    EXPECT_EQ((*it)->getRootPath(), "sweetHome");

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    EXPECT_EQ((locations.front())->getRootPath(), "sweetHome");
}

TEST(RootDirectiveTests, positive_scenarios_wtih_context_different_values)
{
    std::string conf_text = "server {"
                            "root myHome;"
                            "location =/ {"
                            "root sweetHome;"
                            "index index.html;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    Config conf = Config(conf_file_name);

    const std::list<ServerContext*>& servers = conf.getServersList();
    std::list<ServerContext*>::const_iterator it = servers.begin();

    EXPECT_EQ(servers.size(), 1);

    EXPECT_EQ((*it)->getRootPath(), "myHome");

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    EXPECT_EQ((locations.front())->getRootPath(), "sweetHome");
}