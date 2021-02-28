//
// Created by jnannie on 2/22/21.
//

#include "Listener.hpp"
#include "WebServ.hpp"
#include "response/Response.hpp"

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

template <class Key, class Value>
Key max_map_key(const std::map<Key, Value>& map_value) {
    Key max_value = map_value.begin()->first;

    typename std::map<Key, Value>::const_iterator it = map_value.begin();

    while (it != map_value.end()) {
        if (it->first > max_value) {
            max_value = it->first;
        }
        ++it;
    }
    return max_value;


}

void Listener::updateMaxFD(void) {
	int max_tmp = _listener;
	if (!_clients_read.empty()) {
		max_tmp = std::max(max_tmp, max_map_key(_clients_read));
	}
	if (!_clients_write.empty()) {
		max_tmp = std::max(max_tmp, max_map_key(_clients_write));
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

	// by default check to read and write, maybe we will need read|write at same iteration
	_clients_read[sock] = true;
    _clients_write[sock] = true;

	_client_requests[sock] =  new Request();
}

void Listener::processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr) {
    handleRequests(globalReadSetPtr);
	handleResponses(globalWriteSetPtr); // Routing is made inside handling Responses
}

template <class Key, class Value>
typename std::map<Key, Value>::iterator eraseMapElementAndReturnNext(std::map<Key, Value>& map_instance,
                                                             typename std::map<Key, Value>::iterator& it) {
    typename std::map<Key, Value>::iterator next_one = ++it;
    --it;
    map_instance.erase(it);
    return next_one;
}


void Listener::readError(std::map<int, bool>::iterator & it) {
//	close(it->first);
//	delete _client_requests[it->first];
//	it = _clients_read.erase(it);

    close(it->first);
    delete _client_requests[it->first];

    it = eraseMapElementAndReturnNext(_clients_read, it);
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

    const std::string& body = request_obj->getRawBody(); // TODO: body is wrong, headers are removed during parsing so the whole _raw_request is the body
    int length;

    // TODO: NEED CHECKS !!!! SEEMS LIKE SHOULDNT WORK
    std::map<std::string, std::string>::const_iterator it = headers.find("transfer-encoding");
    if ((it != headers.end()) && ((*it).second == "chunked")) {
        size_t      len;
        std::string tmp_body = body;

        while (!tmp_body.empty()) {
            if ((len = libft::strtoul_base(tmp_body, 16)) == 0 && tmp_body.find("0\r\n\r\n") == 0)
                return true;
            if (tmp_body.length() >= len + 5)
                tmp_body = tmp_body.substr(tmp_body.find("\r\n") + 2 + len);
            else
                break;
        }
    }
    else if ((length = request_obj->getHeaderContentLength()) >= 0) {
        std::cout << body.length() << std::endl;
        if (body.length() == static_cast<std::size_t>(length))
            return true;
        return false;
    }
    return true;
}





// returns TRUE if we are ready and need body and its length check, etc.
bool Listener::processHeaderInfoForActions(int client_socket) {
    Request* request = _client_requests[client_socket];

    // we dont need silently change raw request inside parsing methods!
    std::string raw_request_for_parsing = request->getRawRequest();
    request->parseRequestLine(raw_request_for_parsing);
    if (request->isStatusCodeOk()) {
        request->parseHeaders(raw_request_for_parsing);
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

    if (!request->getCountSentResponses()) { // check for header in first iteration
        request->handleExpectHeader();

        if (!request->isStatusCodeOk()) {
            return false;
        }
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

void        set_time(std::map<int, int> &time,
                     std::map<int, bool>::iterator it,
                     std::map<int, bool>::iterator const& ite) {
    while (it != ite) {
        time[it->first] = get_time();
        ++it;
    }
}

void Listener::handleRequests(fd_set* globalReadSetPtr) {
	char buf[BUFFER_LENGHT];
	int bytes_read;
	std::map<int, bool>::iterator it = _clients_read.begin();
	std::map<int, int> time;
	set_time(time, it, _clients_read.end());


	while (it != _clients_read.end() ) {
        int fd = it->first;

        // Поступили данные от клиента, читаем их
        // if fd contains in SELECTed for READ set and it is ready for read(_clients_read[fd] == true)
        if (FD_ISSET(fd, globalReadSetPtr) && it->second)
        {
            Request* request = _client_requests[fd];
            // Check client header info was read or not

            bool header_was_read_client = request->isHeaderWasRead();

            bytes_read = recv(fd, buf, BUFFER_LENGHT - 1, 0);
            if (bytes_read == 0) { // Соединение разорвано, удаляем сокет из множества //
                readError(it); // ERASES iterator instance inside
                continue;
            }
            else if (bytes_read < 0)
                bytes_read = 0;
            else
                time[fd] = get_time();
            buf[bytes_read] = '\0';

            std::cout << "=========================" << std::endl;
            std::cout << buf << std::endl;
            std::cout << std::endl << "=========================" << std::endl;

            try
            {
                request->getRawRequest().append(buf);// собираем строку пока весь запрос не соберем
            }
            catch (std::bad_alloc const& e)
            {
                request->_status_code = 500;
                readError(it);
                continue;
            }

            // Behavior based on was or not read header
            if (!header_was_read_client) {
                bool header_was_read = readAndSetHeaderInfoInRequest(request);
                if (header_was_read) {
                    request->setHeaderWasRead();


                    bool is_continue_read_body = processHeaderInfoForActions(fd);
                    if (!is_continue_read_body && (request->_status_code == 100))
                    {
                        Response* response = new Response(request, fd);
                        response->generateResponse();
                        response->sendResponse();
                        continue ;
                    }

                    if (is_continue_read_body) {
                        bool body_was_read = continueReadBody(_client_requests[fd]);
                        if (body_was_read) {
                            _clients_write[fd] = true;
                            _clients_read[fd] = false;
                            ++it;
//                            it = eraseMapElementAndReturnNext(_clients_read, it);
//                            it = _clients_read.erase(it);
                        }
                        else {
                            ++it;
                        }
                    }
                    else {
                        _clients_write[fd] = true;
                        _clients_read[fd] = false;
                        ++it;
                    }
                }
                else // jnannie: we can read and write only once according to checklist
                    ++it;
            }
            else {
                bool body_was_read = continueReadBody(_client_requests[fd]);
                if (body_was_read) {
                    _clients_write[fd] = true;
                    _clients_read[fd] = false;
                    ++it;
                }
                else {
                    ++it;
                }
            }
        }
        // just skip if not ready
        else {
            if ((get_time() - time[fd]) > TIME_OUT) {
                readError(it);
                continue;
            }
            ++it;
        }
	}
}

void Listener::handleResponses(fd_set* globalWriteSetPtr) {
	std::map<int, bool>::iterator it = _clients_write.begin();

	while (it != _clients_write.end()) {
	    int fd = it->first;

	    // if fd contains in SELECTed for WRITE set and it is ready for write(_clients_write[fd] == true)
		if (FD_ISSET(fd, globalWriteSetPtr) && it->second)
		{
			Request* request = _client_requests[fd];

			// moved to listenere opers
//			request->parseRequestLine();
//			if (request->isStatusCodeOk())
//				request->parseHeaders();

            if (request->_status_code == 100) {
                request->_status_code = 200;
            }


			if (request->isStatusCodeOk()) {
				request->parsURL();
//				std::cout << "request->_request_target: " << request->_request_target << std::endl;
//				if (request->isStatusCodeOk())
//					WebServ::routeRequests(_host, _port, _client_requests);
			}

			request->parseBody();

			Response* response = new Response(request, fd);
			response->generateResponse();
			response->sendResponse();


            delete response;
            close(fd);
            delete _client_requests[fd];
            _client_requests.erase(fd);
//            it = _clients_write.erase(it);
            _clients_read.erase(fd);
//            eraseMapElementAndReturnNext(_clients_read, it);
            it = eraseMapElementAndReturnNext(_clients_write, it);
		}
		else // just skip if not ready
        {
			++it;
		}
	}
}
