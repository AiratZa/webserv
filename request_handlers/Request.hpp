#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>

class MyRequest {

	public:
		MyRequest(const std::string& request ) : _raw_request(request) { };
		~MyRequest(void) { };

		const std::string & getRawRequest(void) { return this->_raw_request; }
		void setRawRequest(const std::string & request) { this->_raw_request = request ;}

	private:
		std::string _raw_request;

};

#endif