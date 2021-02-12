//
// Created by jnannie on 2/12/21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../request_handlers/Request.hpp"
#include <sys/socket.h>

class Response {

public:
	Response(Request *request, int socket) : _raw_response(""), _socket(socket) {
			std::stringstream response; // сюда будет записываться ответ клиенту
			std::stringstream response_body; // тело ответа

			// формируем тело ответа (HTML)
			response_body << "<title>Test C++ HTTP Server</title>\n"
						  << "<h1>Test page</h1>\n"
						  << "<p>This is body of the test page...</p>\n"
						  << "<h2>Request headers</h2>\n"
						  << "<pre>" << request->getRawRequest() << "</pre>\n"
						  << "<em><small>Test C++ Http Server</small></em>\n";

			// Формируем весь ответ вместе с заголовками
			response << "HTTP/1.1 200 OK\r\n"
					 << "Version: HTTP/1.1\r\n"
					 << "Content-Type: text/html; charset=utf-8\r\n"
					 << "Content-Length: " << response_body.str().length()
					 << "\r\n\r\n"
					 << response_body.str();

			_raw_response = response.str();

	};

	~Response(void) { };

	void send_response() {
		// Отправляем ответ клиенту с помощью функции send
			send(_socket, _raw_response.c_str(), _raw_response.length(), 0);
	}


private:
	std::string _raw_response;
	int _socket;

};

#endif //RESPONSE_HPP
