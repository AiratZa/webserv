//
// Created by jnannie on 2/13/21.
//

#include "Request.hpp"

Request::Request(const std::string& request) : _raw_request(request), _status_code(200) { };

Request::~Request(void) { };

	std::string & Request::getRawRequest(void) {
	return this->_raw_request;
}

void Request::setRawRequest(const std::string & request) {
	this->_raw_request = request ;
}

void Request::setStatusCode(int status_code)
{
	_status_code = status_code;
}

void Request::parseRequestLine() {
	size_t word_end = _raw_request.find(' ');
	if (word_end == std::string::npos)
		return setStatusCode(400);
	_method = _raw_request.substr(0, word_end);
	_raw_request.erase(0, word_end + 1);

	word_end = _raw_request.find(' ');
	if (word_end == std::string::npos)
		return setStatusCode(400);
	_request_target = _raw_request.substr(0, word_end);
	_raw_request.erase(0, word_end + 1);

	word_end = _raw_request.find("\r\n");
	if (word_end == std::string::npos
		|| _raw_request.find(' ') < word_end)
		return setStatusCode(400);

	_http_version = _raw_request.substr(0, word_end);
	_raw_request.erase(0, word_end + 2);

	if (_method.length() + _request_target.length() + _http_version.length() + 4 > MAX_REQUEST_LINE_LENGTH)
		return setStatusCode(414); // http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
}

void Request::stringToLower(std::string & str) {
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		*it = libft::tolower(*it);
	}
}

bool Request::isStatusCodeOk() {
	if (_status_code != 200)
		return false;
	return true;
}

void Request::parseChunkedContent() {

}

void Request::getContentByLength() {

}

void Request::parseHeaders() {
	std::string field_name;
	std::string field_value;
	size_t field_name_length;
	size_t field_value_length;

	size_t line_length = _raw_request.find("\r\n");
	while (line_length != 0) {
		if (line_length > MAX_REQUEST_LINE_LENGTH ||
			line_length == std::string::npos) {
			return setStatusCode(400); // http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
		}

		field_name_length = _raw_request.find(':'); // field-name
		if (field_name_length == std::string::npos) {
			return setStatusCode(400);
		}
		field_name = _raw_request.substr(0, field_name_length);

		stringToLower(field_name); // field_name is case-insensitive so we make it lowercase to make life easy

		if (field_name.find(' ') != std::string::npos) { // no spaces inside field-name, rfc 2.3.4
			return setStatusCode(400);
		}
		_raw_request.erase(0, field_name_length + 1);


		field_value_length = line_length - field_name_length - 1; // field-value
		if (_raw_request[0] == ' ') {
			_raw_request.erase(0, 1);
			field_value_length--;
		}
		if (_raw_request[field_value_length - 1] == ' ') {
			_raw_request.erase(field_value_length - 1, 1);
			field_value_length--;
		}
		field_value = _raw_request.substr(0, field_value_length);
		_raw_request.erase(0, field_value_length + 2);

		_headers[field_name] = field_value; // add field_name-field_value to map

		line_length = _raw_request.find("\r\n");
	}
	_raw_request.erase(0, 2);
}

void Request::parse() {
	parseRequestLine();
	if (isStatusCodeOk())
		parseHeaders();

	if (isStatusCodeOk())
	{
		if (_headers.count("transfer-encoding"))
			parseChunkedContent();
		else if (_headers.count("content-length"))
			getContentByLength();
	}
}