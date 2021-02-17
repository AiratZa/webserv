//
// Created by airat on 04.02.2021.
//

#include "Server.hpp"
#include "response/Response.hpp"

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

Server::Server(const ServerContext server_context) {
    const std::map<std::string, std::list<int> >& hosts_n_ports = server_context.getHostsAndPorts();

    _listener = socket(AF_INET, SOCK_STREAM, 0);
    if(_listener < 0)
        utils::exitWithLog();

    if (fcntl(_listener, F_SETFL, O_NONBLOCK) < 0) //превращает сокет в неблокирующий
        utils::exitWithLog();

    int optval = 1;
    if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        utils::exitWithLog();

    std::map<std::string, std::list<int> >::const_iterator map_it = hosts_n_ports.begin();
    std::map<std::string, std::list<int> >::const_iterator map_ite = hosts_n_ports.end();

    while (map_it != map_ite) {
        const std::list<int>& ports = (*map_it).second;
        std::list<int>::const_iterator ports_it = ports.begin();
        std::list<int>::const_iterator ports_ite = ports.end();

        in_addr_t host_addr = getHostInetAddrFromStr((*map_it).first);
        while (ports_it != ports_ite) {
            _addr.sin_family = AF_INET;
            _addr.sin_port = htons((*ports_it));
            _addr.sin_addr.s_addr = host_addr;

            if (bind(_listener, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
                utils::exitWithLog();

            ++ports_it;
        }
        ++map_it;
    }

    if (listen(_listener, SOMAXCONN) < 0)
        utils::exitWithLog();
}


in_addr_t Server::getHostInetAddrFromStr(const std::string& host_str) const {
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


void Server::acceptConnection(void) {
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

int Server::checkFullRequest(std::string const& req) {
    size_t      length;
    int         pointer;

    if (std::string::npos != req.find("\r\n\r\n")) {
        std::string header = req.substr(0, req.find("\r\n\r\n"));
        std::transform(header.begin(), header.end(), header.begin(), tolower); //TODO add stringToLower()
        std::string body = req.substr((req.find("\r\n\r\n") + 4), req.length());
        if (std::string::npos != header.find("transfer-encoding:")) {
            std::string encoding = header.substr(header.find("transfer-encoding:") + 18);
            encoding = encoding.substr(0, encoding.find("\r\n"));
            if (encoding.find("chunked"))
                while (!body.empty()) {
                    if ((length = libft::atoi(body.c_str())) == 0 && body.find("0\r\n\r\n") == 0)
                        return 1;
                    if (body.length() >= length + 5)
                        body = body.substr(body.find("\r\n") + 2 + length);
                    else
                        break;
                }
        }
        else if (std::string::npos != header.find("content-length:")) {
            pointer = header.find("content-length:") + 15;
            length = libft::atoi(header.substr(pointer, header.length()).c_str());
            if (body.length() == length)
                return 1;
        }
        else
            return 1;
    }
    return 0;
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

void Server::handleRequests(fd_set* globalReadSetPtr) {
    char buf[BUFFER_LENGHT];
    int bytes_read;
    std::list<int>::iterator it = _clients_read.begin();
    std::map<int, int> time;
    set_time(time, it, _clients_read.end());

    while (it != _clients_read.end() ) { // TODO we need timeout
        if (FD_ISSET(*it, globalReadSetPtr)) { // Поступили данные от клиента, читаем их
            bytes_read = recv(*it, buf, BUFFER_LENGHT - 1, 0);
            if (bytes_read == 0 || (get_time() - time[*it]) > TIME_OUT) { // Соединение разорвано, удаляем сокет из множества
                close(*it);
                delete _client_requests[*it]; // should use iterator before erasing it
                it = _clients_read.erase(it);
                continue;
            }
            else if (bytes_read < 0)
                bytes_read = 0;
            else
                time[*it] = get_time();
            buf[bytes_read] = '\0';

            _client_requests[*it]->getRawRequest().append(buf);// собираем строку пока весь запрос не соберем

            if (checkFullRequest(_client_requests[*it]->getRawRequest())) { //после последнего считывания проверяем все ли доставлено
                _clients_write.push_back(*it);
                 it = _clients_read.erase(it);
            }
        } else {
            if ((get_time() - time[*it]) > TIME_OUT) {
                close(*it);
                it = _clients_read.erase(it);
                delete _client_requests[*it];
                continue;
            }
            ++it;
        }
    }
    // monkey->SetNext(squirrel)->SetNext(dog);
}

void Server::checkRequest(Request* request) {
	(void)request;
}

void Server::handleResponses(fd_set* globalWriteSetPtr) {
	std::list<int>::iterator it = _clients_write.begin();

	int fd;
	while (it != _clients_write.end()) {
		fd = *it;
		if (FD_ISSET(fd, globalWriteSetPtr)) {
			Request* request = _client_requests[fd];
			request->parse();
			checkRequest(request);

			Response response(request, fd);
			response.sendResponse();

			close(fd);
			_client_requests.erase(fd);
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
    _max_fd = max_tmp;
}


