//
// Created by airat on 14.02.2021.
//

#ifndef WEBSERV_FIXTURES_HPP
#define WEBSERV_FIXTURES_HPP

#define CONFIG_TEST_ROOT_PATH "/home/airat/CLionProjects/webserv/tests/test_configs"
//#define CONFIG_TEST_ROOT_PATH "/Users/gdrake/CLionProjects/webserv_new/config_parsing/test_configs"

void createTestConfigFromString(const std::string& file_name,
                                      const std::string& config_text);


#endif //WEBSERV_FIXTURES_HPP
