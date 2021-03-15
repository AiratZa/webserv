//
// Created by airat on 13.02.2021.
//

#include "gtest/gtest.h"
#include "../../../config_parsing/ServerContext.hpp"
#include "../../../config_parsing/Config.hpp"

////NEGATIVE SCENARIOS

TEST(ServerNameDirectiveTests, more_than_1_asterisk)
{
    Config conf;
    ServerContext serv_context;
    std::list<std::string> serv_names;

    serv_names.push_back("www.hello*");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("*hello.com");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("*.hello.*");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("*.hello*.");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back(".*hello.*");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("***");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();
}

TEST(ServerNameDirectiveTests, quotes_tests)
{
    Config conf;
    ServerContext serv_context;
    std::list<std::string> serv_names;

    serv_names.push_back("\"");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("'");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("\"фаы.qwertyuiop[]asdfghjklzxcvbnm,./\"\"hi\"");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("'фаы.qwertyuiop[]asdfghjklzxcvbnm,./''hi'");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("\"фаы.qwertyuop[]a'1'sdfghjklzxcvbnm\"\"\"");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();

    serv_names.push_back("'фаы.qwertyuop[]a\"1\"sdfghjklzxcvbnm'''");
    EXPECT_THROW(conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names), Config::BadConfigException );
    serv_names.clear();


}




////POSITIVE SCENARIOS HOST

TEST(ServerNameDirectiveTests, one_server_names_in_one_line)
{
    Config conf;

    {
        ServerContext serv_context;
        std::list<std::string> serv_names;

        serv_names.push_back("\"\"");
        conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);

        std::list<std::string> post_serv_names = serv_context.getServerNames();

        ASSERT_EQ(1, post_serv_names.size());

        std::list<std::string>::iterator it = serv_names.begin();
        std::list<std::string>::iterator p_it = post_serv_names.begin();

        while (p_it != post_serv_names.end()) {
            ASSERT_EQ(*p_it, "");
            ++p_it;
            ++it;
        }
    }

    {
        ServerContext serv_context;
        std::list<std::string> serv_names;

        serv_names.push_back("\"  \"");
        conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);

        std::list<std::string> post_serv_names = serv_context.getServerNames();

        ASSERT_EQ(1, post_serv_names.size());

        std::list<std::string>::iterator it = serv_names.begin();
        std::list<std::string>::iterator p_it = post_serv_names.begin();
        while (p_it != post_serv_names.end()) {
            ASSERT_EQ(*p_it, "  ");
            ++p_it;
            ++it;
        }
    }

    {
        ServerContext serv_context;
        std::list<std::string> serv_names;

        serv_names.push_back("''");
        conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);

        std::list<std::string> post_serv_names = serv_context.getServerNames();

        ASSERT_EQ(1, post_serv_names.size());

        std::list<std::string>::iterator it = serv_names.begin();
        std::list<std::string>::iterator p_it = post_serv_names.begin();
        while (p_it != post_serv_names.end()) {
            ASSERT_EQ(*p_it, "");
            ++p_it;
            ++it;
        }
    }

    {
        ServerContext serv_context;
        std::list<std::string> serv_names;

        serv_names.push_back("'  '");
        conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);

        std::list<std::string> post_serv_names = serv_context.getServerNames();

        ASSERT_EQ(1, post_serv_names.size());

        std::list<std::string>::iterator it = serv_names.begin();
        std::list<std::string>::iterator p_it = post_serv_names.begin();
        while (p_it != post_serv_names.end()) {
            ASSERT_EQ(*p_it, "  ");
            ++p_it;
            ++it;
        }
    }

    {
        ServerContext serv_context;
        std::list<std::string> serv_names;

        serv_names.push_back("\"фаы.qwertyuiop[]asdfghjklzxcvbnm,./''hi\"" );
        conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);

        std::list<std::string> post_serv_names = serv_context.getServerNames();

        ASSERT_EQ(1, post_serv_names.size());

        std::list<std::string>::iterator it = serv_names.begin();
        std::list<std::string>::iterator p_it = post_serv_names.begin();
        while (p_it != post_serv_names.end()) {
            ASSERT_EQ(*p_it, "фаы.qwertyuiop[]asdfghjklzxcvbnm,./''hi");
            ++p_it;
            ++it;
        }
    }

    {
        ServerContext serv_context;
        std::list<std::string> serv_names;

        serv_names.push_back("'фаы.qwertyuiop[]asdfghjklzxcvbnm,./\"\"hi'" );
        conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);

        std::list<std::string> post_serv_names = serv_context.getServerNames();

        ASSERT_EQ(1, post_serv_names.size());

        std::list<std::string>::iterator it = serv_names.begin();
        std::list<std::string>::iterator p_it = post_serv_names.begin();
        while (p_it != post_serv_names.end()) {
            ASSERT_EQ(*p_it, "фаы.qwertyuiop[]asdfghjklzxcvbnm,./\"\"hi");
            ++p_it;
            ++it;
        }
    }
}


TEST(ServerNameDirectiveTests, multiple_server_names_in_one_line)
{
    Config conf;
    ServerContext serv_context;
    std::list<std::string> serv_names;

    serv_names.push_back("*.hello1.com");
    serv_names.push_back("");
    serv_names.push_back(".hello3.*");
    serv_names.push_back("www.hello4.com");
    serv_names.push_back("www.hello5.*");

    serv_names.push_back("*.hello1.com");
    serv_names.push_back("");
    serv_names.push_back(".hello3.*");
    serv_names.push_back("www.hello4.com");
    serv_names.push_back("www.hello5.*");
    conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);

    std::list<std::string> post_serv_names = serv_context.getServerNames();

    ASSERT_EQ(serv_names.size()/2, post_serv_names.size());

    std::list<std::string>::iterator it = serv_names.begin();
    std::list<std::string>::iterator p_it = post_serv_names.begin();
    while (p_it != post_serv_names.end()) {
        ASSERT_EQ(*p_it, *it);
        ++p_it;
        ++it;
    }

}



TEST(ServerNameDirectiveTests, multiple_server_names_in_multiple_line)
{
    Config conf;
    ServerContext serv_context;
    std::list<std::string> serv_names;

    serv_names.push_back("*.hello1.com");
    serv_names.push_back("");
    serv_names.push_back(".hello3.*");
    serv_names.push_back("www.hello4.com");
    serv_names.push_back("www.hello5.*");

    serv_names.push_back("*.hello1.com");
    serv_names.push_back("");
    serv_names.push_back(".hello3.*");
    serv_names.push_back("www.hello4.com");
    serv_names.push_back("www.hello5.*");
    conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);
    serv_names.clear();

    serv_names.push_back("*.2hello1.com");
    serv_names.push_back(" ");
    serv_names.push_back(".2hello3.*");
    serv_names.push_back("www.2hello4.com");
    serv_names.push_back("www.2hello5.*");

    serv_names.push_back("*.2hello1.com");
    serv_names.push_back(" ");
    serv_names.push_back(".2hello3.*");
    serv_names.push_back("www.2hello4.com");
    serv_names.push_back("www.2hello5.*");
    conf._checkAndSetParams(&serv_context, &serv_context, SERVER_NAME_KW, serv_names);
    serv_names.clear();



    serv_names.push_back("*.hello1.com");
    serv_names.push_back("");
    serv_names.push_back(".hello3.*");
    serv_names.push_back("www.hello4.com");
    serv_names.push_back("www.hello5.*");

    serv_names.push_back("*.2hello1.com");
    serv_names.push_back(" ");
    serv_names.push_back(".2hello3.*");
    serv_names.push_back("www.2hello4.com");
    serv_names.push_back("www.2hello5.*");


    std::list<std::string> post_serv_names = serv_context.getServerNames();

    ASSERT_EQ(serv_names.size(), post_serv_names.size());

    std::list<std::string>::iterator it = serv_names.begin();
    std::list<std::string>::iterator p_it = post_serv_names.begin();
    while (p_it != post_serv_names.end()) {
        ASSERT_EQ(*p_it, *it);
        ++p_it;
        ++it;
    }

}




