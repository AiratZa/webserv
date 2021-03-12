//
// Created by jnannie on 2/22/21.
//

#include "Listener.hpp"
#include "WebServ.hpp"
#include "response/Response.hpp"
#include "base64_coding/base64.hpp"

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
	_addr.sin_port = htons(port); // TODO: is htons it allowed?
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
		max_tmp = std::max(max_tmp, *std::max_element(_clients_read.begin(), _clients_read.end()));
	}
	if (!_clients_write.empty()) {
		max_tmp = std::max(max_tmp, *std::max_element(_clients_write.begin(), _clients_write.end()));
	}
	_max_fd = max_tmp;
}

long Listener::_get_time(void)
{
	struct timeval current;

	gettimeofday(&current, NULL);
	return ((long)(((current.tv_sec) * 1000) + ((current.tv_usec) / 1000)));
}

void Listener::acceptConnection(void) {
	socklen_t len = sizeof(_remote_addr);
	int sock = accept(_listener, (struct sockaddr *)&_remote_addr, &len);
	if(sock < 0)
		utils::exitWithLog();

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		utils::exitWithLog();

	int optval = 1;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		utils::exitWithLog();

//	_all_clients.push_back(sock);
	_clients_read.push_back(sock);

	_client_requests[sock] =  new Request(_remote_addr, _port);
	_time[sock] = _get_time();
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

        std::size_t raw_request_len = request_obj->_bytes_read;
        if (raw_request_len > (empty_line_pos + 4)) {
            request_obj->shift_from_buf_start = (empty_line_pos + 4);
            request_obj->increaseReadBodySize(raw_request_len - (empty_line_pos + 4));
        }
        return true;
    }
    return false;
}

// return TRUE if length of body is reached else FALSE
bool Listener::continueReadBody(Request* request_obj) {
    const std::map<std::string, std::string>& headers = request_obj->_headers;

//    const std::string& body = request_obj->getRawBody(); // TODO: body is wrong, headers are removed during parsing so the whole _raw_request in the body
    std::string& body = request_obj->getRawRequest();
	long long length;

    // TODO: NEED CHECKS !!!! SEEMS LIKE SHOULDNT WORK
    std::map<std::string, std::string>::const_iterator it = headers.find("transfer-encoding");
    if ((it != headers.end()) && ((*it).second.find("chunked") != std::string::npos)) {
        request_obj->is_chunked = true;

		size_t start_line_length = body.find("\r\n");
		if (start_line_length == std::string::npos)
			return false;

		std::string start_line;
		std::string chunk_length_field;
		size_t chunk_length;
		size_t sum_content_length = 0;

        while (!body.empty()) { // jnannie: remade like Request::parseChunkedContent()

			if (start_line_length == std::string::npos)
				return false;
			if (start_line_length > MAX_HEADER_LINE_LENGTH) {
				request_obj->setStatusCode(400);
				return true;
			}
			start_line = body.substr(0, start_line_length);

			chunk_length_field = start_line.substr(0, body.find(';')); // to ';' or full line

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

//			bool size_check = request_obj->checkToClientMaxBodySize(sum_content_length); // 413 set inside if needed
//			if (!size_check) {
//			    return true; // finished beacuse of SIZE
//			}

			if (body.size() < start_line_length + 2 + chunk_length + 2)
				return false;

			body.erase(0, start_line_length + 2); // remove start line

//			_content.append(_raw_request.substr(0, chunk_length));


			request_obj->_content.append(request_obj->_raw_request.substr(0, chunk_length));

			body.erase(0, chunk_length + 2); // remove rest of chunk

//			std::cout << chunk_length << std::endl;
			if (chunk_length == 0) // why? body is now empty so loop is over
			    return true;

			start_line_length = body.find("\r\n");

//            if ((len = libft::strtoul_base(tmp_body, 16)) == 0 && tmp_body.find("0\r\n\r\n") == 0)
//                return true;
//            if (tmp_body.length() >= len + 5)
//                tmp_body = tmp_body.substr(tmp_body.find("\r\n") + 2 + len);
//            else
//                break;

        }
        return false;
    }
    else if ((request_obj->_headers.count("content-length"))) {
		length = libft::strtoul_base(request_obj->_headers["content-length"], 10);

		bool size_check = request_obj->checkToClientMaxBodySize(length); // 413 set inside if needed
        if (!size_check) {
            return true; // finished beacuse of SIZE
        }

		if (request_obj->getReadBodySize() == length) {
			request_obj->_content.append(body, 0, length);
			body.clear();
			return true;
        }

        return false;
    }
    return true;
}


/*
 * authorization functions to move
 */
std::vector<std::string>    parser_log_pass(std::string file) {
	std::vector<std::string> log_pass;

	int fd_file = open(file.c_str(), O_RDONLY);
	// if (fd_file < 0)
	//     utils::exitWithLog("Error happened when try to open auth_basic_user_file");

	char *str;
	int  read;

	while ((read = get_next_line(fd_file, &str)) == 1) {
		log_pass.push_back(str);
		delete str;
	}
	log_pass.push_back(str);
//	std::cout << "log_pass.size(): " << log_pass.size() << std::endl;
	delete str;

	// if (read != 0)
	//     utils::exitWithLog("Error happened when read auth_basic_user_file");
	close(fd_file);
	return log_pass;
}

bool    find_log_pass(std::vector<std::string> log_pass, std::string const& credentials) {
	std::vector<std::string>::iterator it = log_pass.begin();

	while (it != log_pass.end()) {
		if (Base64::base64_encode(*it) == credentials)
			return true;
		++it;
	}
	return false;
}


// returns TRUE if we are ready and need body and its length check, etc.
bool Listener::processHeaderInfoForActions(int client_socket) {
    Request* request = _client_requests[client_socket];

    std::cout << request->getRawRequest() << std::endl;

    // we dont need silently change raw request inside parsing methods!
    request->parseRequestLine();
    if (request->isStatusCodeOk()) {
        request->parseHeaders();
		if (request->isStatusCodeOk())
			request->parsURL();
		else
			return false;
    }
    else {
        return false;
    }





    WebServ::routeRequest(_host, _port, request);

//    if (request->isMethodLimited(request->_method)) { // TODO: jnannie: its 405, but we check for allowed methods in Response, and set 'location' header if method is not allowed, maybe later move it here, but xz
//        request->_status_code = 403;
//    }


    std::size_t lang_start_pos;
    if ((lang_start_pos = request->_request_target.find("_lang_")) != std::string::npos) {
        std::size_t lang_end_pos = request->_request_target.find_last_of('.');
        lang_start_pos += 6; // pass "_lang_"
        request->_is_lang_file_pos = lang_start_pos;
        std::string lang_code = (request->_request_target).substr(lang_start_pos, (lang_end_pos- lang_start_pos) );
//        if (checkForCorrectLanguageCode(lang_code)) { TODO:
//            request->setReponseContentLang(lang_code);
//        }
    }

    if (!request->isStatusCodeOk()) {
        return false;
    }




    if (request->_handling_location) {
		if (request->_handling_location->getLocationPath() == "/ht") { // TODO: add to config file
			if (request->_headers.count("authorization")) {
				std::vector<std::string> log_pass = parser_log_pass(std::string("base64_coding/passwd"));
				std::string auth_scheme = request->_headers["authorization"].substr(0, 5);
				libft::string_to_lower(auth_scheme);
				std::string credentials = request->_headers["authorization"].substr(6);
				credentials = credentials.substr(credentials.find_first_not_of(" ")); // remove whitespaces
				if (auth_scheme != "basic" || !find_log_pass(log_pass, credentials)) {
					request->setStatusCode(401);
					return false;
				}
			} else {
				request->setStatusCode(401);
				return false;
			}
		}
    }


    request->handleExpectHeader();

    if (!request->isStatusCodeOk()) {
        return false;
    }
    std::cout << request->getAbsoluteRootPathForRequest() << std::endl;

    request->checkToClientMaxBodySize();
    if (!request->isStatusCodeOk()) {
        return false;
    }



    if (request->_method == "PUT") {

        // https://efim360.ru/rfc-7231-protokol-peredachi-giperteksta-http-1-1-semantika-i-kontent/#4-3-4-PUT
        if (request->isConcreteHeaderExists("content-range")) {
            request->_status_code = 400;
            return false;
        }

        std::string target = request->_request_target.substr(1);
        request->_full_filename = request->getAbsolutePathForPUTRequests() + "/" + target;

        bool status = request->isFileExists();
        request->setFileExistenceStatus(status);
        request->setNeedWritingBodyToFile(true);

        if (request->getFileExistenceStatus())
        {
            if (!request->targetIsFile()) {
                if (request->isStatusCodeOk()) {
                    request->_status_code = 409;
                }
                return false;
            }
        }


        if (!request->checkIsMayFileBeOpenedOrCreated())
            return false;
    }

    return true;
}





//void        set_time(std::map<int, int> &time, std::list<int>::iterator it, std::list<int>::iterator const& ite) {
//	while (it != ite) {
//		time[*it] = get_time();
//		++it;
//	}
//}

void Listener::handleRequests(fd_set* globalReadSetPtr) {
	std::list<int>::iterator it = _clients_read.begin();

//	set_time(time, it, _clients_read.end()); //TODO: check if it works


	while (it != _clients_read.end() ) {
        int fd = *it;

		if (FD_ISSET(*it, globalReadSetPtr)) { // Поступили данные от клиента, читаем их
		    // Check client header info was read or not
            Request* request = _client_requests[fd];
            request->setHostAndPort(_host, _port);
            bool header_was_read_client = request->isHeaderWasRead();

            request->_bytes_read = recv(fd, request->_buf, BUFFER_LENGHT - 1, 0);

            if (request->_bytes_read == 0) { // Соединение разорвано, удаляем сокет из множества //
			    readError(it); // ERASES iterator instance inside
				continue;
			}
			else if (request->_bytes_read < 0)
				request->_bytes_read = 0;
			else
				_time[*it] = _get_time();
			request->_buf[request->_bytes_read] = '\0';


            try
            {
                request->getRawRequest().append(request->_buf);// собираем строку пока весь запрос не соберем
                //// FOR TEST! TODO: need delete
                if (request->getRawRequest().find("PUT") !=  std::string::npos) {
                    std::cout << "hey" << std::endl;
                }

                if (header_was_read_client) {
                    request->increaseReadBodySize(request->_bytes_read);
                }
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
                    bool is_continue_read_body = processHeaderInfoForActions(*it);

                    // ONLY WITH EXPECT HEADERS
                    if (is_continue_read_body && (request->_status_code == 100))
                    {
                        Response response = Response(request, fd);
                        response.generateStatusLine();
                        response.sendResponse();

                        continue ;
                    }

                    if (is_continue_read_body) {
                        bool body_was_read = continueReadBody(request);
                        bool writing_to_file_result = true;

                        if (request->getNeedWritingBodyToFile() && body_was_read) {
                            writing_to_file_result = request->writeBodyReadBytesIntoFile();

                            if (!writing_to_file_result) {
                                request->_status_code = 500;
                            }
                        }

                        if (body_was_read || !writing_to_file_result) {
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
                bool writing_to_file_result = true;

                if (request->getNeedWritingBodyToFile() && body_was_read) {
                    writing_to_file_result = request->writeBodyReadBytesIntoFile();

                    if (!writing_to_file_result) {
                        request->_status_code = 500;
                    }
                }

                if (body_was_read || !writing_to_file_result) {
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
			if ((_get_time() - _time[fd]) > TIME_OUT) {
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

//			if (request->isStatusCodeOk()) {
//				request->parsURL();
//				if (request->isStatusCodeOk()) // TODO: routing already done in Listener::handleRequests() -> Listener::processHeaderInfoForActions() so this one is redundant
//					WebServ::routeRequests(_host, _port, _client_requests);
//			}

//			request->parseBody();

			request->checkToClientMaxBodySize(request->_content.size()); // 413 set inside if needed
//			if (!size_check) {
//				return true; // finished beacuse of SIZE
//			}
			Response response(request, fd);
			response.generateResponse();
			response.sendResponse();

//			close(fd);

			if (request->_close_connection) {
				delete _client_requests[fd];
				_client_requests.erase(fd);
				close(fd);
			} else {
				delete _client_requests[fd];
				_client_requests[fd] = new Request(_remote_addr, _port);
				_clients_read.push_back(fd);
			}
//			_client_requests.erase(fd);

//			std::list<int>::iterator it_all = std::find(_all_clients.begin(), _all_clients.end(), fd);
//			if (it_all != _all_clients.end()) {
//                _all_clients.erase(it_all);
//			}

			it = _clients_write.erase(it);

		} else {
			++it;
		}
	}
}
