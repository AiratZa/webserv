//
// Created by airat on 21.02.2021.
//

#include "Server.hpp"

void Server::Listener::acceptConnection(void) {
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

void        Server::Listener::readError(std::list<int>::iterator & it) {
    close(*it);
    delete _client_requests[*it];
    it = _clients_read.erase(it);
}

int Server::Listener::checkFullRequest(std::string const& req) {
    size_t      length;
    int         pointer;

    if (std::string::npos != req.find("\r\n\r\n")) {
        std::string header = req.substr(0, req.find("\r\n\r\n"));
        libft::string_to_lower(header);
        std::string body = req.substr((req.find("\r\n\r\n") + 4), req.length());
        if (std::string::npos != header.find("transfer-encoding:")) {
            std::string encoding = header.substr(header.find("transfer-encoding:") + 18);
            encoding = encoding.substr(0, encoding.find("\r\n"));
            if (encoding.find("chunked"))
                while (!body.empty()) {
                    if ((length = libft::strtoul_base(body, 16)) == 0 && body.find("0\r\n\r\n") == 0)
                        return 1;
                    if (body.length() >= length + 5)
                        body = body.substr(body.find("\r\n") + 2 + length);
                    else
                        break;
                }
        }
        else if (std::string::npos != header.find("content-length:")) {
            pointer = header.find("content-length:") + 15;
            length = libft::strtoul_base(header.substr(pointer, header.length()), 10);
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

void Server::Listener::handleRequests(fd_set* globalReadSetPtr) {
    char buf[BUFFER_LENGHT];
    int bytes_read;
    std::list<int>::iterator it = _clients_read.begin();
    std::map<int, int> time;
    set_time(time, it, _clients_read.end());

    while (it != _clients_read.end() ) {
        if (FD_ISSET(*it, globalReadSetPtr)) { // Поступили данные от клиента, читаем их
            bytes_read = recv(*it, buf, BUFFER_LENGHT - 1, 0);
            if (bytes_read == 0) { // Соединение разорвано, удаляем сокет из множества //
                readError(it);
                continue;
            }
            else if (bytes_read < 0)
                bytes_read = 0;
            else
                time[*it] = get_time();
            buf[bytes_read] = '\0';

            try {
                _client_requests[*it]->getRawRequest().append(buf);// собираем строку пока весь запрос не соберем
            } catch (std::bad_alloc const& e) {
                _client_requests[*it]->_status_code = 500;
                readError(it);
                continue;
            }
            if (checkFullRequest(_client_requests[*it]->getRawRequest())) { //после последнего считывания проверяем все ли доставлено
//                std::cout << _client_requests[*it]->getRawRequest() << std::endl;
                _clients_write.push_back(*it);
                it = _clients_read.erase(it);
            }
            else
                ++it;
        } else {
            if ((get_time() - time[*it]) > TIME_OUT) {
                readError(it);
                continue;
            }
            ++it;
        }
    }
}
