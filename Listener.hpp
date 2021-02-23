//
// Created by jnannie on 2/22/21.
//

#ifndef LISTENER_HPP
#define LISTENER_HPP

class Listener;
#include <string>
#include <list>
#include <sys/socket.h>
#include "request_handlers/Request.hpp"

class Listener {

public:
	Listener(const std::string& host, int port);

	int getListener(void) const { return _listener; }

	const std::list<int>& getReadClients(void) const { return _clients_read; }
	const std::list<int>& getWriteClients(void) const { return _clients_write; }
	void updateMaxFD(void);

	void acceptConnection(void);

	void processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr);
	void handleRequests(fd_set* globalReadSetPtr);
	void handleResponses(fd_set* globalWriteSetPtr);

	const int & getMaxFD() const { return _max_fd; }

	//skarry:
	int     checkFullRequest(std::string const& req);
	void    readError(std::list<int>::iterator & it);


private:
	int _listener;

	struct sockaddr_in _addr;
	const std::string _host;
	const int _port;

	std::list<int> _clients_read;
	std::list<int> _clients_write;
	std::map<int, Request *> _client_requests;

	int _max_fd;

	in_addr_t _getHostInetAddrFromStr(const std::string& host_str) const;
};


#endif //LISTENER_HPP
