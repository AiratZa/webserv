//
// Created by Great Drake on 2/7/21.
//

#ifndef WEBSERV_LIBFT_HPP
#define WEBSERV_LIBFT_HPP

#include <iostream>

namespace libft {
    int isspace(int ch);
    int	atoi(const char *nptr);
    unsigned int unsigned_number_len(unsigned int number, unsigned int base);

    std::string get_next_word(const std::string& value);

	int		tolower(int c);
}

#endif //WEBSERV_LIBFT_HPP
