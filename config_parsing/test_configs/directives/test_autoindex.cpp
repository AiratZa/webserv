//
// Created by airat on 21.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../fixtures.hpp"

//NEGATIVE SCENARIOS AUTOINDEX


TEST(AutoindexDirectiveTests, negtive_with_context_without_params)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "autoindex ;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(AutoindexDirectiveTests, negtive_with_context_invalid_value)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "autoindex 0ff;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(AutoindexDirectiveTests, negtive_with_context_second_declaration)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "autoindex on;"
                            "autoindex on;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

TEST(AutoindexDirectiveTests, negtive_with_context_2_more_params)
{
    std::string conf_text = "server {"
                            "location =/ {"
                            "index index.html;"
                            "autoindex on off;"
                            "}"
                            "}";

    std::string conf_file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/tmp_config.conf";
    createTestConfigFromString(conf_file_name, conf_text);

    EXPECT_THROW({Config conf = Config(conf_file_name);}, Config::BadConfigException );
}

//////POSITIVE SCENARIOS AUTOINDEX

TEST(AliasDirectiveTests, positive_scenarios_wtih_context_default_value)
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

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    bool autoindex_opt;
    autoindex_opt = (*it)->isAutoindexEnabled();
    EXPECT_EQ(autoindex_opt, false);

    autoindex_opt = (*locations.begin())->isAutoindexEnabled();
    EXPECT_EQ(autoindex_opt, false);
}


TEST(AliasDirectiveTests, positive_scenarios_wtih_context_inheritance)
{
    std::string conf_text = "server {"
                            "autoindex on;"
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

    const std::list<LocationContext*>& locations = (*it)->getLocationsList();
    EXPECT_EQ(locations.size(), 1);

    bool autoindex_opt;
    autoindex_opt = (*it)->isAutoindexEnabled();
    EXPECT_EQ(autoindex_opt, true);

    autoindex_opt = (*locations.begin())->isAutoindexEnabled();
    EXPECT_EQ(autoindex_opt, true);
}

TEST(AliasDirectiveTests, positive_scenarios_wtih_context_different_value_in_levels)
{
    std::string conf_text = "server {"
                            "autoindex on;"
                            "location =/ {"
                            "index index.html;"
                            "autoindex off;"
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

    bool autoindex_opt;
    autoindex_opt = (*it)->isAutoindexEnabled();
    EXPECT_EQ(autoindex_opt, true);

    autoindex_opt = (*locations.begin())->isAutoindexEnabled();
    EXPECT_EQ(autoindex_opt, false);
}

TEST(AliasDirectiveTests, positive_scenarios_wtih_context_ON_option)
{
    {
        std::string conf_text = "server {"
                                "autoindex on;"
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

        const std::list<LocationContext*>& locations = (*it)->getLocationsList();
        EXPECT_EQ(locations.size(), 1);

        bool autoindex_opt;
        autoindex_opt = (*it)->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, true);

        autoindex_opt = (*locations.begin())->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, true);
    }

    {
        std::string conf_text = "server {"
                                "autoindex ON;"
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

        const std::list<LocationContext*>& locations = (*it)->getLocationsList();
        EXPECT_EQ(locations.size(), 1);

        bool autoindex_opt;
        autoindex_opt = (*it)->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, true);

        autoindex_opt = (*locations.begin())->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, true);
    }
}

TEST(AliasDirectiveTests, positive_scenarios_wtih_context_OFF_option)
{
    {
        std::string conf_text = "server {"
                                "autoindex off;"
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

        const std::list<LocationContext*>& locations = (*it)->getLocationsList();
        EXPECT_EQ(locations.size(), 1);

        bool autoindex_opt;
        autoindex_opt = (*it)->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, false);

        autoindex_opt = (*locations.begin())->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, false);
    }

    {
        std::string conf_text = "server {"
                                "autoindex OFF;"
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

        const std::list<LocationContext*>& locations = (*it)->getLocationsList();
        EXPECT_EQ(locations.size(), 1);

        bool autoindex_opt;
        autoindex_opt = (*it)->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, false);

        autoindex_opt = (*locations.begin())->isAutoindexEnabled();
        EXPECT_EQ(autoindex_opt, false);
    }
}


