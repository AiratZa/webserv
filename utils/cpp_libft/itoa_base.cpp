//
// Created by jnannie on 2/16/21.
//

#include "libft.h"
#include <string>

static int count_num(size_t n, int base)
{
	int		i;

	i = 1;
	while ((n = n / base) != 0)
		i++;
	return (i);
}

std::string ft_itoa_base(size_t n, int base) // didn't test it
{
	std::string	s;
	int		num;

	num = count_num(n, base);
	while (num--) {
		s[num] = n % base + '0';
		if (s[num] > '9')
			s[num] += 7;
		n /= base;
	}
	return (s);
}
