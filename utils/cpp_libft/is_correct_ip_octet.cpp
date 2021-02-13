//
// Created by airat on 13.02.2021.
//

#include "libft.hpp"

bool libft::is_correct_ip_octet(const std::string& octet_str) {
    size_t len = octet_str.length();
    if ( len <=0 || len > 3 ) {
        return false;
    }
    int nbr = libft::atoi(octet_str.c_str());
    if (nbr < 0 || nbr > 255) {
        return false;
    }
    if (len != libft::unsigned_number_len(nbr, 10)) {
        return false;
    }
    return true;
}

