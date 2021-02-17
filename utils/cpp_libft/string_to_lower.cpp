//
// Created by jnannie on 2/17/21.
//

#include "libft.hpp"
#include <string>

namespace libft {
	void string_to_lower(std::string & str) {
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			*it = libft::tolower(*it);
		}
	}
}
