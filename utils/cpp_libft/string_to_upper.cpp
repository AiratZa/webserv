//
// Created by jnannie on 3/12/21.
//

#include "libft.hpp"
#include <string>

namespace libft {
	void string_to_upper(std::string & str) {
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			*it = libft::toupper(*it);
		}
	}
}