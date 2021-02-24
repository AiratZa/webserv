//
// Created by jnannie on 2/13/21.
//

#include <string>
#include <set>
#include "Request.hpp"
#include "../utils/cpp_libft/libft.hpp"


#define MAX_HEADER_LINE_LENGTH 8192 //http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers

/*
 * CLIENT_MAX_BODY_SIZE is about 1m
 * https://serverfault.com/questions/871717/nginx-disconnect-when-client-sends-chunked-body-exceeding-desired-size
 * https://nginx.org/ru/docs/http/ngx_http_core_module.html#client_max_body_size
 */
//#define CLIENT_MAX_BODY_SIZE 0xfffff

const std::set<std::string> Request::implemented_headers = Request::initRequestHeaders();

std::set<std::string> Request::initRequestHeaders() {
	std::set<std::string> implemented_headers;
	implemented_headers.insert("accept-charset"); // Accept-Charset: utf-8  Accept-Charset: iso-8859-5, unicode-1-1;q=0.8
	implemented_headers.insert("accept-language"); // Accept-Language: ru
	implemented_headers.insert("authorization"); // Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==
	implemented_headers.insert("content-language"); // Content-Language: en, ase, ru
	implemented_headers.insert("content-length"); // Content-Length: 1348
	implemented_headers.insert("content-location");
	implemented_headers.insert("content-type"); // Content-Type: text/html;charset=utf-8
	implemented_headers.insert("date"); // Date: Tue, 15 Nov 1994 08:12:31 GMT
	implemented_headers.insert("host"); // Host: ru.wikipedia.org
	implemented_headers.insert("last-modified");
	implemented_headers.insert("referer"); // Referer: http://en.wikipedia.org/wiki/Main_Page
	implemented_headers.insert("transfer-encoding"); // Transfer-Encoding: gzip, chunked
    implemented_headers.insert("user-agent"); // User-Agent: Mozilla/5.0 (X11; Linux i686; rv:2.0.1) Gecko/20100101 Firefox/4.0.1

    // sent by cURL with PUT request
    implemented_headers.insert("expect"); // Expect: 100-continue
	return implemented_headers;
}

Request::Request()
    : _raw_request(""),
    _status_code(200),
    _client_max_body_size(0xfffff),
    _is_alias_path(false),
      _header_end_pos(0),
      _header_was_read(false) {  };

Request::Request(const std::string& request)
        : _raw_request(request),
        _status_code(200),
        _client_max_body_size(0xfffff),
        _is_alias_path(false),
          _header_end_pos(0),
        _header_was_read(false){ };

Request::~Request(void) { };

std::string & Request::getRawRequest(void) {
	return this->_raw_request;
}

void Request::setRawRequest(const std::string & request) {
	this->_raw_request = request ;
}

void Request::setStatusCode(int status_code) {
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

	if (_method.length() + _request_target.length() + _http_version.length() + 4 > MAX_HEADER_LINE_LENGTH)
		return setStatusCode(414); // http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
}

void Request::parseHeaders() {
	if (!isStatusCodeOk())
		return ;
	std::string field_name;
	std::string field_value;
	size_t field_name_length;
	size_t field_value_length;

	size_t line_length = _raw_request.find("\r\n");
	while (line_length != 0) {
		if (line_length > MAX_HEADER_LINE_LENGTH
			|| line_length == std::string::npos) {
			return setStatusCode(400); // http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
		}

		field_name_length = _raw_request.find(':'); // field-name
		if (field_name_length == std::string::npos) {
			return setStatusCode(400);
		}
		field_name = _raw_request.substr(0, field_name_length);

		libft::string_to_lower(field_name); // field_name is case-insensitive so we make it lowercase to make life easy

		if (field_name.find(' ') != std::string::npos) { // no spaces inside field-name, rfc 2.3.4
			return setStatusCode(400);
		}
		_raw_request.erase(0, field_name_length + 1);


		field_value_length = line_length - field_name_length - 1; // field-value
		if (_raw_request[0] == ' ') {
			_raw_request.erase(0, 1); // remove optional whitespace in the beginning of field-value
			field_value_length--;
		}
		if (_raw_request[field_value_length - 1] == ' ') {
			_raw_request.erase(field_value_length - 1, 1); // remove optional whitespace in the end of field-value
			field_value_length--;
		}
		field_value = _raw_request.substr(0, field_value_length);
		_raw_request.erase(0, field_value_length + 2);

		if (Request::implemented_headers.count(field_name))
		{
			if (_headers.count(field_name)) {
				_headers[field_name].append(",");
			}
			_headers[field_name].append(field_value); // add field_name-field_value to map
		}

		line_length = _raw_request.find("\r\n");
	}
	_raw_request.erase(0, 2);
}

bool Request::isStatusCodeOk() {
	if (_status_code != 200)
		return false;
	return true;
}

/*
 * we ignore trailer according rfc 7230 4.1.2, because our headers dont fit requirements
 */
void Request::parseChunkedContent() {
	std::string chunk_length_field;
	std::string start_line;
	size_t chunk_length;
	size_t sum_content_length = 0;

	size_t start_line_length = _raw_request.find("\r\n");
	while (_raw_request[0] != '0') {
		if (start_line_length > MAX_HEADER_LINE_LENGTH
			|| start_line_length == std::string::npos) {
			return setStatusCode(400);
		}
		start_line = _raw_request.substr(0, start_line_length);

		chunk_length_field = start_line.substr(0, _raw_request.find(';')); // to ';' or full line

		libft::string_to_lower(chunk_length_field);
		chunk_length = libft::strtoul_base(chunk_length_field, 16);
		if (chunk_length == ULONG_MAX)
			return setStatusCode(413); // 413 (Request Entity Too Large)
		sum_content_length +=chunk_length;
		if (_client_max_body_size && sum_content_length > _client_max_body_size)
			return setStatusCode(413);

		_raw_request.erase(0, start_line_length + 2); // remove start line

		_content.append(_raw_request.substr(0, chunk_length));

		_raw_request.erase(0, chunk_length + 2); // remove rest of chunk

		start_line_length = _raw_request.find("\r\n");
	}
	_raw_request.clear();
}

void Request::getContentByLength() {
	size_t content_length = libft::strtoul_base(_headers["content-length"], 10);
	if (content_length == ULONG_MAX)
		return setStatusCode(413); // 413 (Request Entity Too Large)
	if (_client_max_body_size && content_length > _client_max_body_size)
		return setStatusCode(413);
	_content.append(_raw_request.substr(0, content_length));
	_raw_request.erase(0, content_length);
	_raw_request.clear();
}

void Request::parseBody() {
	if (isStatusCodeOk()) {
		if (_headers.count("transfer-encoding")) {
			libft::string_to_lower(_headers["transfer-encoding"]); // to find "chunked"
			if (_headers["transfer-encoding"].find("chunked") != std::string::npos)
				parseChunkedContent();
		}
		else if (_headers.count("content-length"))
			getContentByLength();
		else
			_content.swap(_raw_request);
	}
}



// Routing (Airat)

void Request::setHandlingServer(ServerContext* handling_server){
    _handling_server = handling_server;
}

void Request::setHandlingLocation(LocationContext* location_to_route) {
    _handling_location = location_to_route;
}

std::string    Request::parsURL(std::string url) {
	std::string res;
	std::string tmp;
	std::list<std::string>   path;
	std::list<std::string>::iterator it = path.begin();
	// _requestTarget = url
	int count = 0;
	int lenght = url.length();
	while (count < lenght && url[count] != '/') {
		res += url[count];
		++count;
	}
	while (count < lenght) {
		while (url[count] == '/') // if "//////"
			++count;
		if (url[count] == '.' && // if "/./" of "/."
			(count + 1 == lenght || (count + 1 < lenght && url[count + 1] == '/')))
			count += 1;
		if (url[count] == '.' && // if "/../" or "/.."
			((count + 2 < lenght && url[count + 1] == '.' && url[count + 2] == '/') ||
			 (count + 2 == lenght && url[count + 1] == '.'))) {
			if (!path.empty()) {
				--it;
				path.pop_back();
			}
			count += 2;
		}
		while (count < lenght && url[count] != '/') { //args write
			if (url[count] == '%') {
				char ch;
				if ((ch = libft::percent_decode(url, count)) != '\0') {
					tmp += ch;
					continue;
				}
			}
			tmp += url[count];
			++count;
		}
		if (!tmp.empty()) { // args add
			path.push_back(tmp);
			tmp.clear();
			++it;
		}
	}
	it = path.begin();
	while (it != path.end()) { // uri constructor
		res += '/';
		res += *it;
		++it;
	}
	return res;
}

void Request::setAbsoluteRootPathForRequest(void) {
    std::string globalRootPath = WebServ::getWebServRootPath();
    std::string cont_root_path;

    if (_handling_location) {
        cont_root_path = _handling_location->getAliasPath();
        if (!cont_root_path.empty()) {
            _absolute_root_path_for_request = cont_root_path;
            _is_alias_path = true;
            return ;
        } else {
            cont_root_path = _handling_location->getRootPath() + _handling_location->getLocationPath();
        }
    } else {
        cont_root_path = _handling_server->getRootPath();
    }

    if (cont_root_path[0] == '/') { // if context root path is absolute by itself
        _absolute_root_path_for_request = cont_root_path;
    } else {
        _absolute_root_path_for_request = globalRootPath + cont_root_path;
    }
}

const std::list<std::string>& Request::getIndexPagesListForRequest(void) const {
    if (_handling_location)
        return _handling_location->getIndexPagesDirectiveInfo();
    return _handling_server->getIndexPagesDirectiveInfo();
}



/*
 * return true if METHOD IS NOT ALLOWED BY CONFIG
 * Author: Airat (GDrake)
 */
bool Request::isMethodLimited(const std::string& method) const {
    if (_handling_location) {
        const std::list<std::string> limit_except = (_handling_location)->getLimitExceptMethods();
        if (limit_except.empty())
            return false;

        std::list<std::string>::const_iterator it = limit_except.begin();
        while (it != limit_except.end()) {
            if (method == (*it)) {
                return false;
            }
            ++it;
        }
        return true;
    }
    return false;
}

/*
 * RFC-7231 5.1.1 Expect
 * Author: Airat (GDrake)
 */
void Request::handleExpectHeader(void) {
    const std::map<std::string, std::string>& headers = _headers;

    std::map<std::string, std::string>::const_iterator it = headers.find("expect");
    if ( it != headers.end()) {
        std::string value = (*it).second;
        libft::string_to_lower(value);

        if (value != "100-continue") {
            _status_code = 417;
        } else {
            _status_code = 100;
        }

    }
}