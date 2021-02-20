//
// Created by airat on 21.02.2021.
//

#include "libft.hpp"

long long libft::stoll_base(const std::string& str, int base)
{
    signed long long	nb;
    long	tmp;
    int		i;
    int		minus;

    nb = 0;
    i = 0;
    minus = 0;
    while (isspace((int)str[i]))
        i++;
    if (str[i] == '-')
    {
        minus = 1;
        i++;
    }
    else if (str[i] == '+')
        i++;
    while ((str[i] >= '0') && (str[i] <= '9'))
    {
        tmp = nb;
        nb = nb * 10 + (str[i++] - '0');
        if (nb < tmp) {
            errno = ERANGE;
            return ((minus) ? LLONG_MIN : LLONG_MAX);
        }
    }
    return ((minus) ? (signed long long)(-nb) : (signed long long)(nb));
}