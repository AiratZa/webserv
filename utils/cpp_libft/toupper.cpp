//
// Created by jnannie on 3/12/21.
//

#include "libft.hpp"

namespace libft {
	int		toupper(int c)
	{
		if ((c >= 'a' && c <= 'z'))
			return (c - 32);
		return (c);
	}
}
