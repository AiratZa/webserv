//
// Created by airat on 14.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"
#include "../../ServerContext.hpp"

#include "../fixtures.hpp"

/*TODO
 * error_page a404 =200 /50x.php;
 * invalid value "a404"

    error_page 404 = 200 /50x.php;
    invalid value "="

    error_page 404 =200 500 /50x.php;
    invalid value "=200"


    invalid value "=999999999999999999999999999"


    -1

    invalid



    ===========================================================


    OK

    error_page 404 =200 /50x.php;

    error_page 404 =200 50x.php; //without slash

    long long - MAX VALUE IN NGINX
    999 999 999 999 999 999
 */



////NEGATIVE SCENARIOS

TEST(ErrorPageDirectiveTests, min_max_values_neg)
{
    std::string conf_text = ("server{\n"
                        "\n"
                        "    listen localhost:8085;\n"
                        "    server_name example.com;\n"
                        "    server_name www.example.com;\n"
                        " }");

    std::string file_name = std::string(CONFIG_TEST_ROOT_PATH) + "/directives/tmp_configs/min_max_values_neg_1.conf";
    createTestConfigFromListOfString(file_name, conf_text);

    Config conf(file_name);
//    ServerContext serv_context;
//    std::list<std::string> serv_names;
//
//    serv_names.push_back("*.hello.*");
//    EXPECT_THROW(conf._checkAndSetParams(&serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
//    serv_names.clear();

//    serv_names.push_back("*.hello*.");
//    EXPECT_THROW(conf._checkAndSetParams(&serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
//    serv_names.clear();
//
//    serv_names.push_back(".*hello.*");
//    EXPECT_THROW(conf._checkAndSetParams(&serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
//    serv_names.clear();
//
//    serv_names.push_back("***");
//    EXPECT_THROW(conf._checkAndSetParams(&serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
//    serv_names.clear();
}


////POSITIVE SCENARIOS HOST

//TEST(ServerNameDirectiveTests, one_server_names_in_one_line)
//{
//    Config conf;
//
//    {
//        ServerContext serv_context;
//        std::list<std::string> serv_names;
//
//        serv_names.push_back("\"\"");
//        conf._checkAndSetParams(&serv_context, SERVER_NAME_KW, serv_names);
//
//        std::list<std::string> post_serv_names = serv_context.getServerNames();
//
//        ASSERT_EQ(1, post_serv_names.size());
//
//        std::list<std::string>::iterator it = serv_names.begin();
//        std::list<std::string>::iterator p_it = post_serv_names.begin();
//
//        while (p_it != post_serv_names.end()) {
//            ASSERT_EQ(*p_it, "");
//            ++p_it;
//            ++it;
//        }
//    }
//
//}

