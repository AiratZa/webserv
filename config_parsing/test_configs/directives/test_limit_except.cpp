//
// Created by airat on 20.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../fixtures.hpp"

//NEGATIVE SCENARIOS LIMIT_EXCEPT

TEST(LimitExceptDirectiveTests, negtive_with_context_in_server_level)
{
    std::string conf_text = "server {"
                            "limit_except POST;"
                            "location =/ {"
                            "index index.html;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(LimitExceptDirectiveTests, negtive_with_context_without_params)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "limit_except ;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(LimitExceptDirectiveTests, negtive_with_context_without_param)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "limit_except ;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(LimitExceptDirectiveTests, negtive_with_context_second_declaration)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "limit_except POST;"
                            "limit_except GET;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(LimitExceptDirectiveTests, negtive_with_context_invalid_methods)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "limit_except COPY;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

//POSITIVE SCENARIOS LIMIT_EXCEPT

TEST(LimitExceptDirectiveTests, positive_scenarios_wtih_context_one_allowed_methods)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "limit_except HEAD;"
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

    std::list<std::string> limited_methods = (*locations.begin())->getLimitExceptMethods();

    EXPECT_EQ(limited_methods.size(), 1);
    EXPECT_EQ((*limited_methods.begin()), "HEAD");
}

TEST(LimitExceptDirectiveTests, positive_scenarios_wtih_context_all_allowed_methods)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "limit_except GET HEAD POST PUT DELETE CONNECT OPTIONS TRACE;"
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

    std::list<std::string> limited_methods = (*locations.begin())->getLimitExceptMethods();
    std::list<std::string>::const_iterator it_actual = limited_methods.begin();

    ASSERT_EQ(limited_methods.size(), 8);
    std::string expect[8] = {"GET",
                               "HEAD",
                               "POST",
                               "PUT",
                               "DELETE",
                               "CONNECT",
                               "OPTIONS",
                               "TRACE"};

    for(int i = 0; i < 8; i++) {
        EXPECT_EQ(expect[i], *it_actual);
        ++it_actual;
    }
}

