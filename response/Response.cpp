//
// Created by jnannie on 2/13/21.
//

#include "Response.hpp"
#include <sstream>
#include <string>
#include <set>
#include "../utils/cpp_libft/libft.hpp"

const std::set<std::string> Response::implemented_headers = Response::initResponseHeaders();

const std::map<int, std::string> Response::status_codes = Response::initStatusCodes();

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

Response::Response(Request* request, int socket) : _request(request), _socket(socket), _reason_phrase("OK"), _raw_response(""), _content("") { };

Response::~Response(void) { };

void Response::generateGetResponse() {
	_content += "<title>Test C++ HTTP Server</title>\n";
	_content += "<h1>Test page</h1>\n";
	_content += "<p>This is body of the test page...</p>\n";
	_content += "<h2>Implemented request headers</h2>\n";
	for (std::map<std::string, std::string>::iterator it = _request->_headers.begin(); it != _request->_headers.end(); ++it)
	{
		_content.append("<pre>").append((*it).first).append(":").append((*it).second).append("</pre>\n");
	}
	_content += "<em><small>Test C++ Http Servergg</small></em>\n";


	_raw_response += "HTTP/1.1 ";
	_raw_response += libft::ultostr_base(_request->_status_code, 10);
	_raw_response += " ";
	_raw_response += _reason_phrase;
	_raw_response += "\r\n";
	_raw_response += "Content-Type: text/html; charset=utf-8\r\n";
	_raw_response += "Content-Length: ";
	_raw_response += libft::ultostr_base(_content.length(), 10);
	_raw_response += "\r\n\r\n";
	_raw_response += _content;



//	std::stringstream response; // сюда будет записываться ответ клиенту
//	std::stringstream response_body; // тело ответа
//
//	// формируем тело ответа (HTML)
//	response_body << "<title>Test C++ HTTP Server</title>\n"
//				  << "<h1>Test page</h1>\n"
//				  << "<p>This is body of the test page...</p>\n"
//				  << "<h2>Implemented request headers</h2>\n";
//	for (std::map<std::string, std::string>::iterator it = _request->_headers.begin(); it != _request->_headers.end(); ++it)
//	{
//		response_body << "<pre>" << (*it).first << ":" << (*it).second << "</pre>\n";
//	}
//	response_body << "<em><small>Test C++ Http Servergg</small></em>\n";
//
//	// Формируем весь ответ вместе с заголовками
//	response << "HTTP/1.1 200 OK\r\n"
//			 << "Version: HTTP/1.1\r\n"
//			 << "Content-Type: text/html; charset=utf-8\r\n"
//			 << "Content-Length: " << response_body.str().length()
//			 << "\r\n\r\n"
//			 << response_body.str();
//
//	_raw_response = response.str();
}

void Response::generateHeadResponse() {

}

void Response::generatePutResponse() {

}

void Response::generateResponse() {
	if (_request->_method == "GET") {
		generateGetResponse();
	} else if (_request->_method == "HEAD") {
		generateHeadResponse();
	} else if (_request->_method == "PUT") {
		generatePutResponse();
	} else {
		return _request->setStatusCode(501); // 501 Not Implemented
	}
}

void Response::sendResponse() {
	// Отправляем ответ клиенту с помощью функции send
	send(_socket, _raw_response.c_str(), _raw_response.length(), 0);
}