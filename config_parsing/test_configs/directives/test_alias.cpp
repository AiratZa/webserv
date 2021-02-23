//
// Created by airat on 21.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../fixtures.hpp"

//NEGATIVE SCENARIOS INDEX

TEST(AliasDirectiveTests, negtive_with_context_in_server_level)
{
    std::string conf_text = "server {"
                            "alias POST;"
                            "location =/ {"
                            "index index.html;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(AliasDirectiveTests, negtive_with_context_without_params)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "alias ;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}


TEST(AliasDirectiveTests, negtive_with_context_second_declaration)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "alias /1;"
                            "alias /2;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(AliasDirectiveTests, negtive_with_context_2_more_params)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "alias /1 /2;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

////POSITIVE SCENARIOS LOCATION CONTEXT

TEST(AliasDirectiveTests, positive_scenarios_wtih_context)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "alias \"/home\";"
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

    std::string alias = (*locations.begin())->getAliasPath();

    EXPECT_EQ(alias, "/home");
}
