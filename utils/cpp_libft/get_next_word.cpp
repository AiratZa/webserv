//
// Created by airat on 08.02.2021.
//

#include "libft.hpp"

std::string libft::get_next_word(const std::string& value) {
    std::string str = "";

    int i = 0;

    bool quote_found = false;
    char first_quote;
    if ((value[i] == '\'') || (value[i] == '"')) {
        quote_found = true;
        first_quote = (value[i] == '\'') ? '\'' : '"';
        i++;
    }
    if (!quote_found)
    {
        while (value[i] && \
            !isspace(value[i]) &&\
            value[i] != '{' && \
            value[i] != '}' && \
            value[i] != ';') {

            str += value[i];
            i++;
        }
    }
    else
    {
        str += first_quote;

        while (value[i] && (value[i] != first_quote)) {
            str += value[i];
            i++;
        }

        if (value[i] == first_quote)
            str += first_quote;
    }

    return str;

}
