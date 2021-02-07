//
// Created by Great Drake on 2/7/21.
//

#include "libft.hpp"

int libft::isspace(int ch) {
    if (ch == ' ' ||
            ch == '\f' ||
            ch == '\n' ||
            ch == '\r' ||
            ch == '\t' ||
            ch == '\v')
        return (1);
    return (0);
}
