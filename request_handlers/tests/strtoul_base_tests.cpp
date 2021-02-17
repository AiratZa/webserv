//
// Created by jnannie on 2/17/21.
//

#include "gtest/gtest.h"
#include "../../WebServ.hpp"
#include "../../utils/cpp_libft/libft.hpp"

#include <string>

TEST(StrtoulBase, strtoul_base_tests_hex) {
	ASSERT_EQ(libft::strtoul_base("f", 16), 15);
	ASSERT_EQ(libft::strtoul_base("ffffffffffffffff", 16), 0xffffffffffffffff);
	ASSERT_EQ(libft::strtoul_base("ffffffffffffffff1", 16), 0xffffffffffffffff);
	ASSERT_EQ(libft::strtoul_base("r", 16), 0);
	ASSERT_EQ(libft::strtoul_base("", 16), 0);
	ASSERT_EQ(libft::strtoul_base("0", 16), 0);
}

TEST(StrtoulBase, strtoul_base_tests_dec) {
	ASSERT_EQ(libft::strtoul_base("18446744073709551615", 10), 18446744073709551615);
	ASSERT_EQ(libft::strtoul_base("18446744073709551615123", 10), 18446744073709551615);
	ASSERT_EQ(libft::strtoul_base("f", 10), 0);
	ASSERT_EQ(libft::strtoul_base("", 10), 0);
}
