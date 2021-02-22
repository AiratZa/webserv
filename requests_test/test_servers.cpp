

#include "gtest/gtest.h"
#include "../WebServ.hpp"
#include "../response/Response.hpp"

#include <string>

TEST(Chunked, ReqestsTest) {

    Server* one = new Server(8080);

    ASSERT_EQ(one->checkFullRequest("hi\r\ntransfer-encoding:gzip,chunked\r\n\r\n10\r\n12345678980\r\n\r\n"), 1);
}

TEST(Content_Length, ReqestsTest) {

    Server* one = new Server(8080);

    ASSERT_EQ(one->checkFullRequest("hi\r\ncontent-length:10\r\n\r\n1234567890"), 1);
}

TEST(URL, ReqestsTest) {
    ASSERT_EQ(parsURL((std::string) "github.com/./AiratZa/webserv"),
              (std::string) "github.com/AiratZa/webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/../AiratZa/webserv"),
              (std::string) "github.com/AiratZa/webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/..//////AiratZa/////webserv"),
              (std::string) "github.com/AiratZa/webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/..//////AiratZa/..////webserv"),
              (std::string) "github.com/webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/.././/.//./AiratZa/../../AiratZa///webserv"),
              (std::string) "github.com/AiratZa/webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/.././/.//./AiratZa/../../AiratZa///webserv/."),
              (std::string) "github.com/AiratZa/webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/.././/.//./AiratZa/../../AiratZa///webserv/./"),
              (std::string) "github.com/AiratZa/webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/.././/.//./AiratZa/../"),
              (std::string) "github.com");
    ASSERT_EQ(parsURL((std::string) "github.com/.././/.//./AiratZa/../../AiratZa///webserv/.."),
              (std::string) "github.com/AiratZa");


//    procents
    ASSERT_EQ(parsURL((std::string) "github.com/./AiratZa/%84webserv"),
              (std::string) "github.com/AiratZa/%84webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/./AiratZa/%20webserv"),
              (std::string) "github.com/AiratZa/ webserv");
    ASSERT_EQ(parsURL((std::string) "github.com/./AiratZa/%20webserv%"),
              (std::string) "github.com/AiratZa/ webserv%");
}
