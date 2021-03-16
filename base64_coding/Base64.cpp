#include "Base64.hpp"

/*
 * https://tools.ietf.org/html/rfc4648#section-4
 */
const std::string base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Base64::base64_encode(std::string input) {
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

std::string Base64::base64_decode(std::string input) {
	std::string output;
	std::string str_24_bits;
	char ch1, ch2, ch3, ch4;

	while (!input.empty()) {
		str_24_bits = input.substr(0, 4);
		ch1 = static_cast<char>(base64_alphabet.find(str_24_bits[0]));
		ch2 = static_cast<char>(base64_alphabet.find(str_24_bits[1]));
		if (str_24_bits[2] != '=')
			ch3 = static_cast<char>(base64_alphabet.find(str_24_bits[2]));
		else
			ch3 = '\0';
		if (str_24_bits[3] != '=')
			ch4 = static_cast<char>(base64_alphabet.find(str_24_bits[3]));
		else
			ch4 = '\0';
		output.append(1, (ch1 << 2) + (ch2 >> 4 & 0x3));
		output.append(1, ((ch2 & 0xf) << 4) + (ch3 >> 2 & 0xf));
		output.append(1, ((ch3 & 0x3) << 6) + ch4);

		input.erase(0, 4);
	}
	return output;
}
