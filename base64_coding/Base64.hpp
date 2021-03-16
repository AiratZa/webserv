//
//  base64 encoding and decoding with C++.
//  Version: 2.rc.08 (release candidate)
//

#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>

class Base64 {
public:
	static std::string base64_encode(std::string input);
	static std::string base64_decode(std::string input);
};


#endif