//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_LIBFT_HPP
#define WEBSERV_LIBFT_HPP

#include <iostream>
#include <string>
#include <climits>
#include <cerrno>

namespace libft {
    int isspace(int ch);
    int	isdigit(int c);

    int	atoi(const char *nptr);
    unsigned int unsigned_number_len(unsigned long long number, unsigned int base);

    bool is_correct_ip_octet(const std::string& octet_str);

    std::string get_next_word(const std::string& value);

	int		tolower(int c);

    unsigned long strtoul_base(std::string str, int base);
    long long stoll_base(const std::string& str, int base);
	void string_to_lower(std::string & str);
	std::string ultostr_base(unsigned long n, int base);
}

#endif //WEBSERV_LIBFT_HPP