//
//  base64 encoding and decoding with C++.
//  Version: 2.rc.08 (release candidate)
//

#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>

std::string base64_encode(std::string const& s, bool url = false);
std::string base64_decode(std::string const& s, bool remove_linebreaks = false);
std::string base64_encode(unsigned char const*, size_t len, bool url = false);


#endif