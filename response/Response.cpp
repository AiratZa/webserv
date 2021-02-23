//
// Created by jnannie on 2/13/21.
//

#include "Response.hpp"
#include <sstream>
#include <string>
#include <set>
#include "../utils/cpp_libft/libft.hpp"

std::set<std::string> Response::implemented_headers = Response::initResponseHeaders();

std::map<int, std::string> Response::status_codes = Response::initStatusCodes();

std::set<std::string> Response::initResponseHeaders() {
	std::set<std::string> implemented_headers;
	implemented_headers.insert("allow"); // Allow: OPTIONS, GET, HEAD
	implemented_headers.insert("content-language"); // Content-Language: en, ase, ru
	implemented_headers.insert("content-length"); // Content-Length: 1348
	implemented_headers.insert("content-location");
	implemented_headers.insert("content-type"); // Content-Type: text/html;charset=utf-8
	implemented_headers.insert("date"); // Date: Tue, 15 Nov 1994 08:12:31 GMT
	implemented_headers.insert("last-modified");
	implemented_headers.insert("location"); // Location: http://example.com/about.html#contacts
	implemented_headers.insert("retry-after");
	implemented_headers.insert("server"); // Server: Apache/2.2.17 (Win32) PHP/5.3.5
	implemented_headers.insert("transfer-encoding"); // Transfer-Encoding: gzip, chunked
	implemented_headers.insert("www-authenticate");
	return implemented_headers;
}

std::map<int,std::string> Response::initStatusCodes() {
	std::map<int,std::string> status_codes;
	status_codes[100] = "CONTINUE";
	status_codes[101] = "SWITCHING PROTOCOLS";
	status_codes[200] = "OK";
	status_codes[201] = "CREATED";
	status_codes[202] = "ACCEPTED";
	status_codes[203] = "NON-AUTHORITATIVE INFORMATION";
	status_codes[204] = "NO CONTENT";
	status_codes[205] = "RESET CONTENT";
	status_codes[206] = "PARTIAL CONTENT";
	status_codes[300] = "MULTIPLE CHOICES";
	status_codes[301] = "MOVED PERMANENTLY";
	status_codes[302] = "FOUND";
	status_codes[303] = "SEE OTHER";
	status_codes[304] = "NOT MODIFIED";
	status_codes[305] = "USE PROXY";
	status_codes[307] = "TEMPORARY REDIRECT";
	status_codes[400] = "BAD REQUEST";
	status_codes[401] = "UNAUTHORIZED";
	status_codes[402] = "PAYMENT REQUIRED";
	status_codes[403] = "FORBIDDEN";
	status_codes[404] = "NOT FOUND";
	status_codes[405] = "METHOD NOT ALLOWED";
	status_codes[406] = "NOT ACCEPTABLE";
	status_codes[407] = "PROXY AUTHENTICATION REQUIRED";
	status_codes[408] = "REQUEST TIMEOUT";
	status_codes[409] = "CONFLICT";
	status_codes[410] = "GONE";
	status_codes[411] = "LENGTH REQUIRED";
	status_codes[412] = "PRECONDITION FAILED";
	status_codes[413] = "PAYLOAD TOO LARGE";
	status_codes[414] = "URI TOO LONG";
	status_codes[415] = "UNSUPPORTED MEDIA TYPE";
	status_codes[416] = "RANGE NOT SATISFIABLE";
	status_codes[417] = "EXPECTATION FAILED";
	status_codes[426] = "UPGRADE REQUIRED";
	status_codes[500] = "UPGRADE REQUIRED";
	status_codes[501] = "NOT IMPLEMENTED";
	status_codes[502] = "BAD GATEWAY";
	status_codes[503] = "SERVICE UNAVAILABLE";
	status_codes[504] = "GATEWAY TIMEOUT";
	status_codes[505] = "HTTP VERSION NOT SUPPORTED";
	return status_codes;
}

Response::Response(Request* request, int socket) :
				_request(request), _socket(socket), _status_code(request->_status_code),
				_reason_phrase("OK"), _raw_response(""), _content(""), _root(WebServ::getWebServRootPath()) { };

Response::~Response(void) { };


bool writeFileContentToString(const std::string& file_name, std::string& content) {
    int file = open(file_name.c_str(), O_RDONLY);
    if (file == -1)
        return false;

    // reading file line by line
    char *str;
    int ret;

    while ((ret = get_next_line(file, &str)) == 1) {
        content += str;
        delete str;
    }
    content += str;
    delete str;
    close(file);
    return true;
}

bool Response::setIndexFileContentToResponseContent(void) {
    std::string root_path = _request->getAbsoluteRootPathForRequest() + _request->_request_target;

    const std::list<std::string>& index_pages = _request->getIndexPagesListForRequest();

    std::list<std::string>::const_iterator it = index_pages.begin();

    std::string file_content = "";
    while (it != index_pages.end()) {
        std::string file_name = root_path + (*it);
        if (writeFileContentToString(file_name, file_content)) {
            _content += file_content;
            return true;
        }
        ++it;
    }
    return false;
}

//void Response::generateGetResponse() {
//	std::cout << "TARGET: " << _request->_request_target << std::endl;
//	std::cout << "ABS PATH: " << _request->getAbsoluteRootPathForRequest() << std::endl;
//	setIndexFileContentToResponseContent();
//}

void Response::generateStatusLine() {
	_raw_response += "HTTP/1.1 ";
	_raw_response += libft::ultostr_base(_status_code, 10);
	_raw_response += " ";
	_raw_response += Response::status_codes[_status_code];
	_raw_response += "\r\n";
}

void Response::generateHeaders() {
//	_raw_response += "Content-Type: text/html; charset=utf-8\r\n";
	_raw_response += _content_type;
	_raw_response += "Content-Length: ";
	_raw_response += libft::ultostr_base(_content.length(), 10);
	_raw_response += "\r\n\r\n";
}

void Response::generateResponseByStatusCode() {
	_content_type = "Content-Type: text/html; charset=utf-8\r\n";
	_content.append(libft::ultostr_base(_status_code, 10)).append(" ").append(Response::status_codes[_status_code]);

	generateStatusLine();
	generateHeaders();
	_raw_response.append(_content);
}


bool Response::isStatusCodeOk() {
	if (_status_code != 200)
		return false;
	return true;
}

void Response::readFileToContent(std::string & filename) {
	char buf[1024 + 1];
	int ret;
	int fd;

	fd = open(filename.c_str(), O_RDONLY);
	while ((ret = read(fd, buf, 1024))) {
		if (ret < 0)
			utils::exitWithLog();
		_content.append(buf, ret);
	}
}

void Response::generateAutoindex() { // TODO:replace by normal autoindex
	_content = "generated autoindex";
}

void Response::setContentTypeByFilename(std::string & filename) {
	std::string ext;
	size_t dot_pos;

	dot_pos = filename.rfind('.');
	if (dot_pos == std::string::npos)
		_content_type = "Content-Type: application/octet-stream\r\n";
	else {
		ext = filename.substr(dot_pos);
		if (ext == ".txt")
			_content_type = "Content-Type: text/plain; charset=utf-8\r\n";
		else if (ext == ".html")
			_content_type = "Content-Type: text/html; charset=utf-8\r\n";
		else if (ext == ".jpg")
			_content_type = "Content-Type: image/jpeg; charset=utf-8\r\n";
		else
			_content_type = "Content-Type: application/octet-stream\r\n";
	}
}

void Response::generateGetResponse() {
	struct stat stat_buf;
	std::string filename;

//	std::cout << "_request->getAbsoluteRootPathForRequest() " << _request->getAbsoluteRootPathForRequest() << std::endl;
//	std::cout << "_request->_handling_location->getAliasPath() " << _request->_handling_location->getAliasPath() << std::endl;
//	std::cout << "_request->_handling_location->getLocationPath() " << _request->_handling_location->getLocationPath() << std::endl;
//	std::cout << "_request->_handling_location->getRootPath() " << _request->_handling_location->getRootPath() << std::endl;
//	std::cout << "_request->_handling_location->getLocationPathForComparison() " << _request->_handling_location->getLocationPathForComparison() << std::endl;
//	std::cout << "_request->_handling_server->getRootPath() " << _request->_handling_server->getRootPath() << std::endl;

	filename = _root + _request->_request_target;

	if (stat(filename.c_str(), &stat_buf) == 0) { // file or directory exists
		if (S_ISDIR(stat_buf.st_mode)) { // filename is a directory
			std::list<std::string> index_list = _request->_handling_server->getIndexPagesDirectiveInfo();
			if (!index_list.empty()) {
				for (std::list<std::string>::const_iterator it = index_list.begin(); it != index_list.end(); ++it) {
					filename += *it;
					if (stat(filename.c_str(), &stat_buf) == 0) {
						break ;
					}
				}
			}
		}
		if (S_ISREG(stat_buf.st_mode)) {
			readFileToContent(filename);
			setContentTypeByFilename(filename);
		} else if (S_ISDIR(stat_buf.st_mode)) {
			if (_request->_handling_location && _request->_handling_location->isAutoindexEnabled()) {
				generateAutoindex();
				_content_type = "Content-Type: text/html; charset=utf-8\r\n";
			} else {
				_status_code = 403;
			}
		} else {
			_status_code = 403; // TODO:check what code to return if file is not a directory and not a regular file
		}
	} else {
		_status_code = 404;
	}

	if (!isStatusCodeOk())
		return ;
//		return generateResponseByStatusCode();

	generateStatusLine();
	generateHeaders();
	_raw_response += _content;
}

void Response::generateHeadResponse() {

}

void Response::generatePutResponse() {

}

void Response::generateResponse() {
	if (_request->isStatusCodeOk()) {
		if (_request->_method == "GET") {
			generateGetResponse();
		} else if (_request->_method == "HEAD") {
			generateHeadResponse();
		} else if (_request->_method == "PUT") {
			generatePutResponse();
		} else {
			_status_code = 501; // 501 Not Implemented
		}
	}
	if (!isStatusCodeOk()) {
		generateResponseByStatusCode();
	}
}

void Response::sendResponse() {
	// Отправляем ответ клиенту с помощью функции send
	send(_socket, _raw_response.c_str(), _raw_response.length(), 0);
}
