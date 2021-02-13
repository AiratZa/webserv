//
// Created by jnannie on 2/13/21.
//

#include "Response.hpp"
#include <sstream>

Response::Response(Request* request, int socket) : _raw_response(""), _request(request), _socket(socket) { };

Response::~Response(void) { };

void Response::sendResponse() {
	std::stringstream response; // сюда будет записываться ответ клиенту
	std::stringstream response_body; // тело ответа

	// формируем тело ответа (HTML)
	response_body << "<title>Test C++ HTTP Server</title>\n"
				  << "<h1>Test page</h1>\n"
				  << "<p>This is body of the test page...</p>\n"
				  << "<h2>Request headers</h2>\n";
	for (std::map<std::string, std::string>::iterator it = _request->_headers.begin(); it != _request->_headers.end(); ++it)
	{
		response_body << "<pre>" << (*it).first << ":" << (*it).second << "</pre>\n";
	}
	response_body << "<em><small>Test C++ Http Server</small></em>\n";

	// Формируем весь ответ вместе с заголовками
	response << "HTTP/1.1 200 OK\r\n"
			 << "Version: HTTP/1.1\r\n"
			 << "Content-Type: text/html; charset=utf-8\r\n"
			 << "Content-Length: " << response_body.str().length()
			 << "\r\n\r\n"
			 << response_body.str();

	_raw_response = response.str();

	// Отправляем ответ клиенту с помощью функции send
	send(_socket, _raw_response.c_str(), _raw_response.length(), 0);
}