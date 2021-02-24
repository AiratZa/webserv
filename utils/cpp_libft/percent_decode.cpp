//
// Created by Saturn Karry on 2/22/21.
//

#include <vector>
#include "libft.hpp"

/*
 * TODO: any code with % should be substituted with char
 */
char			libft::percent_decode(std::string const& str, int & start)
{
    std::string value[18] = {
            "%20",
            "%22",
            "%25",
            "%0A",
            "%0D",
            "%0D0A",
            "%2D",
            "%2E",
            "%3C",
            "%3E",
            "%5C",
            "%5E",
            "%5F",
            "%60",
            "%7B",
            "%7C",
            "%7D",
            "%7E"
    };
    std::string ch(" \"%\n\n\n-.<>\\^_`{|}~");
    std::string to_find;
    if (str.length() - start > 5) {
        for (int i = 0; i < 5; i++) {
            to_find += str[start + i];
        }
        if (value[5] == to_find) {
            start += 5;
            return ch[5];
        }
    }
    if (str.length() - start > 3) {
        to_find.clear();
        for (int i = 0; i < 3; i++) {
            to_find += str[start + i];
        }
        for (int i = 0; i < 18; i++) {
            if (value[i] == to_find) {
                start += 3;
                return ch[i];
            }
        }
    }
    return ('\0');
}


