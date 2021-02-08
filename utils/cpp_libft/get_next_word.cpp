//
// Created by airat on 08.02.2021.
//

#include "libft.hpp"

std::string libft::get_next_word(const std::string& value) {
    std::string str = "";

    int i = 0;
    while (value[i] && \
            !isspace(value[i]) &&\
            value[i] != '{' && \
            value[i] != '}' && \
            value[i] != ';') {

        str += value[i];
        i++;
    }

    return str;

}
