//
// Created by jnannie on 2/18/21.
//

#include "gtest/gtest.h"
#include "../../WebServ.hpp"
#include "../../utils/cpp_libft/libft.hpp"

#include <string>

TEST(UltostrBase, ultostr_base_tests_hex) {
	ASSERT_EQ(libft::ultostr_base(0xffffffffffffffff, 16), "ffffffffffffffff");
	ASSERT_EQ(libft::ultostr_base(0, 16), "0");
	ASSERT_EQ(libft::ultostr_base(-1, 16), "ffffffffffffffff");
	ASSERT_EQ(libft::ultostr_base(1, 16), "1");
	ASSERT_EQ(libft::ultostr_base(0xf, 16), "f");
}

TEST(UltostrBase, ultostr_base_tests_dec) {
	ASSERT_EQ(libft::ultostr_base(18446744073709551615UL, 10), "18446744073709551615");
	ASSERT_EQ(libft::ultostr_base(0xf, 10), "15");
	ASSERT_EQ(libft::ultostr_base(0, 10), "0");
	ASSERT_EQ(libft::ultostr_base(-1, 10), "18446744073709551615");
}
