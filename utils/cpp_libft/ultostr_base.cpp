//
// Created by jnannie on 2/18/21.
//

#include "libft.hpp"
#include <string>

namespace libft {
	static int count_num(unsigned long n, int base)
	{
		int i;

		i = 1;
		while ((n = n / base) != 0)
			i++;
		return (i);
	}

	/*
	 * only 10 and 16 bases
	 * 16 bases in lowercase
	 */
	std::string ultostr_base(unsigned long n, int base)
	{
		std::string s;
		int num;

		num = count_num(n, base);
		s.resize(num);
		while (num--) {
			s[num] = n % base + '0';
			if (s[num] > '9')
				s[num] += 39; // distance between '9' and 'a'
			n /= base;
		}
		return (s);
	}
}
