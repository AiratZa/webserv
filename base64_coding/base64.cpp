#include "base64.hpp"

/*
 * https://tools.ietf.org/html/rfc4648#section-4
 */
char const base64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Base64::base64_encode(std::string & input) {
	std::string output;
	std::string str_24_bits;

	while (!input.empty()) {
		str_24_bits = input.substr(0, 3);
		if (str_24_bits.size() == 3) {
			output += base64_alphabet[str_24_bits[0] >> 2 & 0x3f];
			output += base64_alphabet[((str_24_bits[0] & 0x3) << 4) + (str_24_bits[1] >> 4 & 0xf)];
			output +=base64_alphabet[((str_24_bits[1] & 0xf) << 2) + (str_24_bits[2] >> 6 & 0x3)];
			output +=base64_alphabet[str_24_bits[2] & 0x3f];
		} else if (str_24_bits.size() == 2) {
			output += base64_alphabet[str_24_bits[0] >> 2 & 0x3f];
			output += base64_alphabet[((str_24_bits[0] & 0x3) << 4) + (str_24_bits[1] >> 4 & 0xf)];
			output +=base64_alphabet[((str_24_bits[1] & 0xf) << 2) + (str_24_bits[2] >> 6 & 0x3)];
			output += "=";
		} else  {
			output += base64_alphabet[str_24_bits[0] >> 2 & 0x3f];
			output += base64_alphabet[((str_24_bits[0] & 0x3) << 4) + (str_24_bits[1] >> 4 & 0xf)];
			output += "=";
			output += "=";
		}

		input.erase(0, 3);
	}

	return output;
}
