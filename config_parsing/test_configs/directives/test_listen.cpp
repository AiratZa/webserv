//
// Created by airat on 10.02.2021.
//

#include "gtest/gtest.h"
#include "../../Config.hpp"

TEST(ListenDirectiveTests,testOne)
{
    Config tmp;
    std::string host = "1localhost";

    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
                     try
                     {
                         tmp.parseHost(host);
                     }
                     catch( const Config::BadConfigException& e )
                     {
                         // and this tests that it has the correct message
//                         EXPECT_STREQ( "Cucumber overflow", e.what() );
                         throw;
                     }
                 }, Config::BadConfigException );

    ASSERT_EQ(5,5);
}
