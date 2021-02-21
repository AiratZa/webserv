//
// Created by airat on 21.02.2021.
//

#include "Server.hpp"

void Server::Listener::checkRequest(Request* request) {
    if (request->isStatusCodeOk()) {
        request->checkMethod();
        request->checkRequestTarget();
        request->checkHttpVersion();
        request->checkHeaders();
    }
}


void Server::Listener::handleResponses(fd_set* globalWriteSetPtr) {
    std::list<int>::iterator it = _clients_write.begin();

    int fd;
    while (it != _clients_write.end()) {
        fd = *it;
        if (FD_ISSET(fd, globalWriteSetPtr)) {
            Request* request = _client_requests[fd];
            request->parse();
            checkRequest(request);

            Response response(request, fd);
            response.generateResponse();
            response.sendResponse();

            close(fd);
            _client_requests.erase(fd);
            it = _clients_write.erase(it);

        } else {
            ++it;
        }
    }
}