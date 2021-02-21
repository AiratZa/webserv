//
// Created by airat on 21.02.2021.
//

#include "gtest/gtest.h"
#include "../../cpp_libft/libft.hpp"

TEST(Stoll_base_tests, max_value_plus_1_neg)
{
    errno = 0;
    ASSERT_EQ(libft::stoll_base("9223372036854775808", 10), LLONG_MAX);
    ASSERT_EQ(errno, ERANGE);
}

TEST(Stoll_base_tests, min_value_minus_1_neg)
{
    errno = 0;
    ASSERT_EQ(libft::stoll_base("-9223372036854775809", 10), LLONG_MIN);
    ASSERT_EQ(errno, ERANGE);
}

TEST(Stoll_base_tests, zero_pos)
{
    errno = 0;
    ASSERT_EQ(libft::stoll_base("0", 10), 0);
    ASSERT_EQ(errno != ERANGE, true);
}

TEST(Stoll_base_tests, max_value_pos)
{
    errno = 0;
    ASSERT_EQ(libft::stoll_base("9223372036854775807", 10), LLONG_MAX);
    ASSERT_EQ(errno != ERANGE, true);
}

TEST(Stoll_base_tests, min_value_pos)
{
    errno = 0;
    ASSERT_EQ(libft::stoll_base("-9223372036854775808", 10), LLONG_MIN);
    ASSERT_EQ(errno != ERANGE, true);
}