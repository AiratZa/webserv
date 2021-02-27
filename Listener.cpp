//
// Created by jnannie on 2/22/21.
//

#include "Listener.hpp"
#include "WebServ.hpp"
#include "response/Response.hpp"

#define MAX_HEADER_LINE_LENGTH 8192 //http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers TODO:look if we should use it from config

Listener::Listener(const std::string &host, int port)
		: _host(host), _port(port)
{
	_listener = socket(AF_INET, SOCK_STREAM, 0);
	if(_listener < 0)
		utils::exitWithLog();

	if (fcntl(_listener, F_SETFL, O_NONBLOCK) < 0) //превращает сокет в неблокирующий
		utils::exitWithLog();

	int optval = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		utils::exitWithLog();

	in_addr_t host_addr = _getHostInetAddrFromStr(host);

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = host_addr;


	if (bind(_listener, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		utils::exitWithLog();

	if (listen(_listener, SOMAXCONN) < 0)
		utils::exitWithLog();
}

in_addr_t Listener::_getHostInetAddrFromStr(const std::string& host_str) const {
	in_addr_t host_addr;
	if (host_str == "*") {
		host_addr = htonl(INADDR_ANY);
	} else if (host_str == "localhost") {
		host_addr = htonl(INADDR_LOOPBACK);
	} else {
		host_addr = inet_addr(host_str.c_str());
	}
	return host_addr;
}

void Listener::updateMaxFD(void) {
	int max_tmp = _listener;
	if (!_clients_read.empty()) {
		max_tmp = std::max(max_tmp, *std::max_element(_clients_read.begin(), _clients_read.end()));
	}
	if (!_clients_write.empty()) {
		max_tmp = std::max(max_tmp, *std::max_element(_clients_write.begin(), _clients_write.end()));
	}
	_max_fd = max_tmp;
}

void Listener::acceptConnection(void) {
	int sock = accept(_listener, NULL, NULL);
	if(sock < 0)
		utils::exitWithLog();

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		utils::exitWithLog();

	int optval = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		utils::exitWithLog();

	_clients_read.push_back(sock);

	_client_requests[sock] =  new Request();
}

void Listener::processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr) {
	handleRequests(globalReadSetPtr);
	handleResponses(globalWriteSetPtr); // Routing is made inside handling Responses
}

void Listener::readError(std::list<int>::iterator & it) {
	close(*it);
	delete _client_requests[*it];
	_client_requests.erase(*it);
	it = _clients_read.erase(it);
}

// return TRUE if header was read else FALSE
bool Listener::readAndSetHeaderInfoInRequest(Request* request_obj) {
    const std::string& raw_request = request_obj->getRawRequest();

    std::size_t empty_line_pos = raw_request.find("\r\n\r\n");
    if (std::string::npos != empty_line_pos) {
        request_obj->setHeaderEndPos(empty_line_pos);
        return true;
    }
    return false;
}

// return TRUE if length of body is reached else FALSE
bool Listener::continueReadBody(Request* request_obj) {
    const std::map<std::string, std::string>& headers = request_obj->_headers;

//    const std::string& body = request_obj->getRawBody(); // TODO: body is wrong, headers are removed during parsing so the whole _raw_request is the body
    const std::string& body = request_obj->getRawRequest();
    int length;

    // TODO: NEED CHECKS !!!! SEEMS LIKE SHOULDNT WORK
    std::map<std::string, std::string>::const_iterator it = headers.find("transfer-encoding");
    if ((it != headers.end()) && ((*it).second == "chunked")) {
//        size_t      len;
        std::string tmp_body = body;


		size_t start_line_length = tmp_body.find("\r\n");

		std::string start_line;
		std::string chunk_length_field;
		size_t chunk_length;
		size_t sum_content_length = 0;
		size_t client_max_body_size = request_obj->_handling_server->getClientMaxBodySizeInfo();

        while (!tmp_body.empty()) { // jnannie: remade like Request::parseChunkedContent()

			if (start_line_length == std::string::npos)
				return false;
			if (start_line_length > MAX_HEADER_LINE_LENGTH) {
				request_obj->setStatusCode(400);
				return true;
			}
			start_line = tmp_body.substr(0, start_line_length);

			chunk_length_field = start_line.substr(0, tmp_body.find(';')); // to ';' or full line

			libft::string_to_lower(chunk_length_field);
			if (chunk_length_field.find_first_not_of("0123456789abcdef") != std::string::npos) {
				request_obj->setStatusCode(400);
				return true;
			}
			chunk_length = libft::strtoul_base(chunk_length_field, 16);
			if (chunk_length == ULONG_MAX || chunk_length > ULONG_MAX - sum_content_length) {
				request_obj->setStatusCode(413);// 413 (Request Entity Too Large)
				return true;
			}
			sum_content_length += chunk_length;
			if (client_max_body_size && sum_content_length > client_max_body_size) {
				request_obj->setStatusCode(413);// 413 (Request Entity Too Large)
				return true;
			}

			tmp_body.erase(0, start_line_length + 2); // remove start line

//			_content.append(_raw_request.substr(0, chunk_length));

			if (tmp_body.size() < chunk_length + 2)
				return false;

			tmp_body.erase(0, chunk_length + 2); // remove rest of chunk

			start_line_length = tmp_body.find("\r\n");

//            if ((len = libft::strtoul_base(tmp_body, 16)) == 0 && tmp_body.find("0\r\n\r\n") == 0)
//                return true;
//            if (tmp_body.length() >= len + 5)
//                tmp_body = tmp_body.substr(tmp_body.find("\r\n") + 2 + len);
//            else
//                break;

        }
    }
    else if ((length = request_obj->getHeaderContentLength()) >= 0) {
        if (body.length() == static_cast<std::size_t>(length))
            return true;
        return false;
    }
    return true;
}





// returns TRUE if we are ready and need body and its length check, etc.
bool Listener::processHeaderInfoForActions(int client_socket) {
    Request* request = _client_requests[client_socket];

    request->parseRequestLine();
    if (request->isStatusCodeOk()) {
        request->parseHeaders();
        if (!request->isStatusCodeOk()) {
            return false;
        }
    }
    else {
        return false;
    }

    WebServ::routeRequest(_host, _port, request);
    if (request->isMethodLimited(request->_method)) {
        request->_status_code = 403;
    }

    if (!request->isStatusCodeOk()) {
        return false;
    }

    request->handleExpectHeader();

    if (!request->isStatusCodeOk()) {
        return false;
    }

    return true;
}

int			get_time(void)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((int)(((current.tv_sec) * 1000) + ((current.tv_usec) / 1000)));
}

void        set_time(std::map<int, int> &time, std::list<int>::iterator it, std::list<int>::iterator const& ite) {
	while (it != ite) {
		time[*it] = get_time();
		++it;
	}
}

void Listener::handleRequests(fd_set* globalReadSetPtr) {
	char buf[BUFFER_LENGHT];
	int bytes_read;
	std::list<int>::iterator it = _clients_read.begin();
	std::map<int, int> time;
	set_time(time, it, _clients_read.end());


	while (it != _clients_read.end() ) {

		if (FD_ISSET(*it, globalReadSetPtr)) { // Поступили данные от клиента, читаем их
		    // Check client header info was read or not

		    bool header_was_read_client = _client_requests[*it]->isHeaderWasRead();

			bytes_read = recv(*it, buf, BUFFER_LENGHT - 1, 0);
			if (bytes_read == 0) { // Соединение разорвано, удаляем сокет из множества //
				readError(it); // ERASES iterator instance inside
				continue;
			}
			else if (bytes_read < 0)
				bytes_read = 0;
			else
				time[*it] = get_time();
			buf[bytes_read] = '\0';

			try
			{
				_client_requests[*it]->getRawRequest().append(buf);// собираем строку пока весь запрос не соберем
			}
			catch (std::bad_alloc const& e)
			{
				_client_requests[*it]->_status_code = 500;
				readError(it);
				continue;
			}

			// Behavior based on was or not read header
			if (!header_was_read_client) {
			    bool header_was_read = readAndSetHeaderInfoInRequest(_client_requests[*it]);
			    if (header_was_read) {
                    _client_requests[*it]->setHeaderWasRead();
                    bool is_continue_read_body = processHeaderInfoForActions(*it);

                    if (is_continue_read_body) {
                        bool body_was_read = continueReadBody(_client_requests[*it]);
                        if (body_was_read) {
                            _clients_write.push_back(*it);
                            it = _clients_read.erase(it);
                        }
                        else {
                            ++it;
                        }
                    }
                    else {
                        _clients_write.push_back(*it);
                        it = _clients_read.erase(it);
                    }
                }
			    else // jnannie: we can read and write only once according to checklist
			    	++it;
            }
			else {
			    bool body_was_read = continueReadBody(_client_requests[*it]);
			    if (body_was_read) {
                    _clients_write.push_back(*it);
                    it = _clients_read.erase(it);
			    }
			    else {
                    ++it;
                }
			}
		}
		// if not ready for reading (socket not in SET)
		else {
			if ((get_time() - time[*it]) > TIME_OUT) {
				readError(it);
				continue;
			}
			++it;
		}
	}
}

void Listener::handleResponses(fd_set* globalWriteSetPtr) {
	std::list<int>::iterator it = _clients_write.begin();

	int fd;
	while (it != _clients_write.end()) {
		fd = *it;
		if (FD_ISSET(fd, globalWriteSetPtr)) {
			Request* request = _client_requests[fd];

			// moved to listenere opers
//			request->parseRequestLine();
//			if (request->isStatusCodeOk())
//				request->parseHeaders();

			if (request->isStatusCodeOk()) {
				request->parsURL();
//				if (request->isStatusCodeOk()) // TODO: routing already done in Listener::handleRequests() -> Listener::processHeaderInfoForActions() so this one is redundant
//					WebServ::routeRequests(_host, _port, _client_requests);
			}

			request->parseBody();

			Response response(request, fd);
			response.generateResponse();
			response.sendResponse();

			close(fd);
			delete _client_requests[fd];
			_client_requests.erase(fd);
			it = _clients_write.erase(it);

		} else {
			++it;
		}
	}
}
