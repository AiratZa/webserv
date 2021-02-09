//
// Created by airat on 09.02.2021.
//

#include "libft.hpp"

unsigned int libft::unsigned_number_len(unsigned int number, unsigned int base) {
    unsigned int number_of_digits = 0;

    do {
        ++number_of_digits;
        number /= base;
    } while (number);
    return number_of_digits;
}

