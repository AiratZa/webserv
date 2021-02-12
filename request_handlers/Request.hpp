#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>

#define MAX_REQUEST_LINE_LENGTH 8192

class Request {

	public:
		Request(const std::string& request ) : _raw_request(request) { };
		~Request(void) { };

		const std::string & getRawRequest(void) { return this->_raw_request; }
		void setRawRequest(const std::string & request) { this->_raw_request = request ;}

		void parseRequestLine() {
			size_t end = _raw_request.find("\r\n");

			if (end >= MAX_REQUEST_LINE_LENGTH)
				_status_code = 414;
		}

		void parseHeaders() {

		}

		void parse() {
			parseRequestLine();
			parseHeaders();
		}

	private:
		std::string _raw_request;

		int _status_code;

//		bool _is_chunked;
//		bool _is_content_lenght;
//		size_t _content_lenght;

};

#endif