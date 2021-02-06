//
// Created by airat on 04.02.2021.
//

#include "Server.hpp"

Server::Server(int port) : _port(port) {

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	_listener = socket(AF_INET, SOCK_STREAM, 0);
	if(_listener < 0)
		utils::exitWithLog();

	if (fcntl(_listener, F_SETFL, O_NONBLOCK) < 0) //превращает сокет в неблокирующий
		utils::exitWithLog();

	int optval = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		utils::exitWithLog();

	if (bind(_listener, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		utils::exitWithLog();

	if (listen(_listener, SOMAXCONN) < 0)
		utils::exitWithLog();

};

void Server::acceptConnection(void) {
	int sock = accept(_listener, NULL, NULL);
	if(sock < 0)
		utils::exitWithLog();

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		utils::exitWithLog();

//	int optval = 1;
//	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
//		utils::exitWithLog();

	_clients_read.push_back(sock);
}

void Server::handleRequests(fd_set* globalReadSetPtr) {
	char buf[1024];
	int bytes_read;
	std::list<int>::iterator it = _clients_read.begin();

	while (it != _clients_read.end()) {
		if (FD_ISSET(*it, globalReadSetPtr)) {
			// Поступили данные от клиента, читаем их
			bytes_read = recv(*it, buf, 1024, 0);

			if (bytes_read <= 0) {
				// Соединение разорвано, удаляем сокет из множества
				close(*it);
				it = _clients_read.erase(it);
				continue;
			}

			// Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
			// В буфере запроса.
			buf[bytes_read] = '\0';

			std::string tmp = buf;
			MyRequest* tmpRequest = new MyRequest(tmp);
			_client_requests[*it] = tmpRequest;
			_clients_write.push_back(*it);
			it = _clients_read.erase(it);
		} else {
			++it;
		}
	}
	// monkey->SetNext(squirrel)->SetNext(dog);
}

void Server::handleResponses(fd_set* globalWriteSetPtr) {
	std::list<int>::iterator it = _clients_write.begin();

	while (it != _clients_write.end()) {
		if (FD_ISSET(*it, globalWriteSetPtr)) {
			std::stringstream response; // сюда будет записываться ответ клиенту
			std::stringstream response_body; // тело ответа

			// формируем тело ответа (HTML)
			response_body << "<title>Test C++ HTTP Server</title>\n"
						  << "<h1>Test page</h1>\n"
						  << "<p>This is body of the test page...</p>\n"
						  << "<h2>Request headers</h2>\n"
						  << "<pre>" << _client_requests[*it]->getRawRequest() << "</pre>\n"
						  << "<em><small>Test C++ Http Server</small></em>\n";

			// Формируем весь ответ вместе с заголовками
			response << "HTTP/1.1 200 OK\r\n"
					 << "Version: HTTP/1.1\r\n"
					 << "Content-Type: text/html; charset=utf-8\r\n"
					 << "Content-Length: " << response_body.str().length()
					 << "\r\n\r\n"
					 << response_body.str();

			// Отправляем ответ клиенту с помощью функции send
			send(*it, response.str().c_str(),
				 response.str().length(), 0);
			close(*it);
			_client_requests.erase(*it);
			it = _clients_write.erase(it);

		} else {
			++it;
		}
	}
	// monkey->SetNext(squirrel)->SetNext(dog);
}



void Server::processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr) {
	handleRequests(globalReadSetPtr);
	handleResponses(globalWriteSetPtr);
}

void Server::updateMaxFD(void) {
    int max_tmp = _listener;
    if (!_clients_read.empty()) {
        max_tmp = std::max(max_tmp, *std::max_element(_clients_read.begin(), _clients_read.end()));
    }
    if (!_clients_write.empty()) {
        max_tmp = std::max(max_tmp, *std::max_element(_clients_write.begin(), _clients_write.end()));
    }
    _maxFD = max_tmp;
}


