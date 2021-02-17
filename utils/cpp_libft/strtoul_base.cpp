//
// Created by jnannie on 2/17/21.
//

#include <string>
#include <climits>
#include <iostream>

namespace libft {
	static int digit_count(std::string & str, int base)
	{
		int i;

		i = 0;
		while (((str[i] >= '0') && (str[i] <= '9'))
			   || (base == 16 && (str[i] >= 'a') && (str[i] <= 'f')))
			i++;
		return (i);
	}

	/*
	 * function skips whitespaces and zeros
	 * 10 and 16 base only
	 * 16 base numbers must be in lowercase
	 * dont forget to set base properly
	 */
	unsigned long strtoul_base(std::string str, int base)
	{
		unsigned long res;
		int n;
		unsigned int digit;

		str.erase(0, str.find_first_not_of("\t\n\r\v\f "));
		str.erase(0, str.find_first_not_of('0'));
		res = 0;
		n = digit_count(str, base);

		for (int i = 0; i < n; i++) {
			if ((str[i] >= '0') && (str[i] <= '9'))
				digit = str[i] - '0';
			else
				digit = 10 + (str[i] - 'a');

			if (res < ULONG_MAX / base)
				res = res * base + digit;
			else if (res == ULONG_MAX / base) {
				if (digit > ULONG_MAX % base)
					return ULONG_MAX;
				res = res * base + digit;
			}
			else
				return ULONG_MAX;
		}
		return (res);
	}
}

