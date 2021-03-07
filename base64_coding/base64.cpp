#include "base64.hpp"

//#include <algorithm>
//#include <stdexcept>

/*
 * https://tools.ietf.org/html/rfc4648#section-4
 */
char const base64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Base64::base64_encode(std::string & input) {
	std::string output;
	int int_24_bits;
	std::string str_24_bits;

	while (!input.empty()) {
		str_24_bits = input.substr(0, 3);
		int_24_bits = *(reinterpret_cast<const int *>(str_24_bits.c_str()));
		if (str_24_bits.size() == 3) {
			output += base64_alphabet[int_24_bits >> 26];
			output += base64_alphabet[int_24_bits << 6 >> (26)];
			output += base64_alphabet[int_24_bits << 12 >> (26)];
			output += base64_alphabet[int_24_bits << 18 >> (26)];
		} else if (str_24_bits.size() == 2) {
			output += base64_alphabet[int_24_bits >> 26];
			output += base64_alphabet[int_24_bits << 6 >> (26)];
			output += base64_alphabet[int_24_bits << 12 >> (26)];
			output += "=";
		} else  {
			output += base64_alphabet[int_24_bits >> 2 & 0x3f];
			output += base64_alphabet[(int_24_bits << 4) & 0x3f];
			output += "=";
			output += "=";
		}

		input.erase(0, 3);
	}

	return output;
}

//static std::string Base64::base64_decode(std::string & input) {
//
//}


//static const char* base64_chars[2] = {
//             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//             "abcdefghijklmnopqrstuvwxyz"
//             "0123456789"
//             "+/",
//
//             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//             "abcdefghijklmnopqrstuvwxyz"
//             "0123456789"
//             "-_"};
//
//static unsigned int pos_of_char(const unsigned char chr) {
// //
// // Return the position of chr within base64_encode()
// //
//
//    if      (chr >= 'A' && chr <= 'Z') return chr - 'A';
//    else if (chr >= 'a' && chr <= 'z') return chr - 'a' + ('Z' - 'A') + 1;
//    else if (chr >= '0' && chr <= '9') return chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
//    else if (chr == '+' || chr == '-') return 62;
//    else if (chr == '/' || chr == '_') return 63;
//    else
//
//    throw std::runtime_error("Input is not valid base64-encoded data.");
//}
//
//template <typename String>
//static std::string encode(String s, bool url) {
//  return base64_encode(reinterpret_cast<const unsigned char*>(s.data()), s.length(), url);
//}
//
//std::string base64_encode(unsigned char const* bytes_to_encode, size_t in_len, bool url) {
//
//    size_t len_encoded = (in_len +2) / 3 * 4;
//
//    unsigned char trailing_char = url ? '.' : '=';
//
//    const char* base64_chars_ = base64_chars[url];
//
//    std::string ret;
//    ret.reserve(len_encoded);
//
//    unsigned int pos = 0;
//
//    while (pos < in_len) {
//        ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0xfc) >> 2]);
//
//        if (pos+1 < in_len) {
//           ret.push_back(base64_chars_[((bytes_to_encode[pos + 0] & 0x03) << 4) + ((bytes_to_encode[pos + 1] & 0xf0) >> 4)]);
//
//           if (pos+2 < in_len) {
//              ret.push_back(base64_chars_[((bytes_to_encode[pos + 1] & 0x0f) << 2) + ((bytes_to_encode[pos + 2] & 0xc0) >> 6)]);
//              ret.push_back(base64_chars_[  bytes_to_encode[pos + 2] & 0x3f]);
//           }
//           else {
//              ret.push_back(base64_chars_[(bytes_to_encode[pos + 1] & 0x0f) << 2]);
//              ret.push_back(trailing_char);
//           }
//        }
//        else {
//
//            ret.push_back(base64_chars_[(bytes_to_encode[pos + 0] & 0x03) << 4]);
//            ret.push_back(trailing_char);
//            ret.push_back(trailing_char);
//        }
//
//        pos += 3;
//    }
//
//
//    return ret;
//}
//
//template <typename String>
//static std::string decode(String encoded_string, bool remove_linebreaks) {
//
//    if (encoded_string.empty()) return std::string();
//
//    if (remove_linebreaks) {
//
//       std::string copy(encoded_string);
//
//       copy.erase(std::remove(copy.begin(), copy.end(), '\n'), copy.end());
//
//       return base64_decode(copy, false);
//    }
//
//    size_t length_of_string = encoded_string.length();
//    size_t pos = 0;
//    size_t approx_length_of_decoded_string = length_of_string / 4 * 3;
//    std::string ret;
//    ret.reserve(approx_length_of_decoded_string);
//
//    while (pos < length_of_string) {
//
//       size_t pos_of_char_1 = pos_of_char(encoded_string[pos+1] );
//
//    //
//    // Emit the first output byte that is produced in each chunk:
//    //
//       ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char(encoded_string[pos+0]) ) << 2 ) + ( (pos_of_char_1 & 0x30 ) >> 4)));
//
//       if ( ( pos + 2 < length_of_string  )       &&
//              encoded_string[pos+2] != '='        &&
//              encoded_string[pos+2] != '.'
//          )
//       {
//          unsigned int pos_of_char_2 = pos_of_char(encoded_string[pos+2] );
//          ret.push_back(static_cast<std::string::value_type>( (( pos_of_char_1 & 0x0f) << 4) + (( pos_of_char_2 & 0x3c) >> 2)));
//
//          if ( ( pos + 3 < length_of_string )     &&
//                 encoded_string[pos+3] != '='     &&
//                 encoded_string[pos+3] != '.'
//             )
//          {
//             ret.push_back(static_cast<std::string::value_type>( ( (pos_of_char_2 & 0x03 ) << 6 ) + pos_of_char(encoded_string[pos+3])   ));
//          }
//       }
//
//       pos += 4;
//    }
//
//    return ret;
//}
//
//std::string base64_decode(std::string const& s, bool remove_linebreaks) {
//   return decode(s, remove_linebreaks);
//}
//
//std::string base64_encode(std::string const& s, bool url) {
//   return encode(s, url);
//}
