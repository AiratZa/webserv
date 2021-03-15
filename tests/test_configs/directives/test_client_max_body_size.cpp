//
// Created by airat on 21.02.2021.
//

#include "gtest/gtest.h"
#include "../../../config_parsing/Config.hpp"
#include "../fixtures.hpp"

//NEGATIVE SCENARIOS CLIENT_MAX_BODY_SIZE

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_zero_args)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size ;"
                            "location =/ {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_1_plus_args)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size 10 10;"
                            "location =/ {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_duplicate)
{
    std::string conf_text = "server {"
                            "client_max_body_size 10;"
                            "index index.html;"
                            "client_max_body_size 10;"
                            "location =/ {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_unknown_measure_unit)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size 10a;"
                            "location =/ {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_nefative_value)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size -10;"
                            "location =/ {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_max_value_bytes)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size 9223372036854775808;"
                            "location =/ {"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_max_value_kbytes)
{
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 9007199254740992k;"
                                "location =/ {"
                                "}"
                                "}";

        std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
        createTestConfigFromString(conf_file_name, conf_text);

        EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
    }
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 9007199254740992K;"
                                "location =/ {"
                                "}"
                                "}";

        std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
        createTestConfigFromString(conf_file_name, conf_text);

        EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
    }
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_max_value_mbytes)
{
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8796093022208m;"
                                "location =/ {"
                                "}"
                                "}";

        std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
        createTestConfigFromString(conf_file_name, conf_text);

        EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
    }
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8796093022208M;"
                                "location =/ {"
                                "}"
                                "}";

        std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
        createTestConfigFromString(conf_file_name, conf_text);

        EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
    }
}

TEST(ClientMaxBodySizeDirectiveTests, negative_scenarios_with_config_context_max_value_gbytes)
{
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8589934592g;"
                                "location =/ {"
                                "}"
                                "}";

        std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
        createTestConfigFromString(conf_file_name, conf_text);

        EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
    }
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8589934592G;"
                                "location =/ {"
                                "}"
                                "}";

        std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
        createTestConfigFromString(conf_file_name, conf_text);

        EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
    }
}

//POSITIVE SCENARIOS CLIENT_MAX_BODY_SIZE

TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_wtih_context_default_value)
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

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), 1024*1024);
    EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), 1024*1024);
}

TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_wtih_context_zero_value_turn_off)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size 0;"
                            "location =/abc {"
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

    EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), 0);
    EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), 0);
}

TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_wtih_context_inheritance)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size 8589934591G;"
                            "location =/abc {"
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

    long long gb = 1024*1024*1024;
    long long expected_value = gb*8589934591;

    EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
    EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value);
}

TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_wtih_context_different_values)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size 8589934591G;"
                            "location =/abc {"
                            "client_max_body_size 8589934591m;"
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

    long long gb = 1024*1024*1024;
    long long expected_value = gb*8589934591;

    EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
    EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value/1024);
}


TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_with_config_context_max_value_bytes)
{
    std::string conf_text = "server {"
                            "index index.html;"
                            "client_max_body_size 9223372036854775807;"
                            "location =/ {"
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


    EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), LLONG_MAX);
    EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), LLONG_MAX);
}


TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_with_config_context_max_value_kbytes)
{
    long long expected_value = 9007199254740991 * 1024;
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 9007199254740991k;"
                                "location =/ {"
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


        EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
        EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value);
    }

    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 9007199254740991K;"
                                "location =/ {"
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


        EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
        EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value);
    }
}

TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_with_config_context_max_value_mbytes)
{
    long long expected_value = 8796093022207 * (1024*1024);
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8796093022207m;"
                                "location =/ {"
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


        EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
        EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value);
    }

    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8796093022207M;"
                                "location =/ {"
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


        EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
        EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value);
    }
}

TEST(ClientMaxBodySizeDirectiveTests, positive_scenarios_with_config_context_max_value_gbytes)
{
    long long expected_value = 8589934591 * (1024*1024*1024);
    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8589934591g;"
                                "location =/ {"
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


        EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
        EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value);
    }

    {
        std::string conf_text = "server {"
                                "index index.html;"
                                "client_max_body_size 8589934591G;"
                                "location =/ {"
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


        EXPECT_EQ((*it)->getClientMaxBodySizeInfo(), expected_value);
        EXPECT_EQ((locations.front())->getClientMaxBodySizeInfo(), expected_value);
    }
}

