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
#include "response/Response.hpp"

class Listener {

public:
	Listener(const std::string& host, int port);

	~Listener(void);

	int getListener(void) const { return _listener; }

	const std::list<int>& getReadClients(void) const { return _clients_read; }
	const std::list<int>& getWriteClients(void) const { return _clients_write; }
//    const std::list<int>& getAllClients(void) const {
//	    return _all_clients;
//	}

	void updateMaxFD(void);

	void acceptConnection(void);

	void processConnections(fd_set* globalReadSetPtr, fd_set* globalWriteSetPtr);
	void handleRequests(fd_set* globalReadSetPtr);
	void handleResponses(fd_set* globalWriteSetPtr);

	const int & getMaxFD() const { return _max_fd; }

	//skarry:
	int     checkFullRequest(std::string const& req);
    void readError(std::list<int>::iterator & it);


        // Airat (GDrake)
    bool readAndSetHeaderInfoInRequest(Request* request_obj);
    bool continueReadBody(Request* request_obj);
    bool processHeaderInfoForActions(int client_socket);

private:
	int _listener;

	struct sockaddr_in _addr;
	const std::string _host;
	const int _port;

	struct sockaddr_in _remote_addr;

//	std::list<int> _all_clients;
	std::list<int> _clients_read; // second param for ready or not
	std::list<int> _clients_write; // second param for ready or not
	std::map<int, Request *> _client_requests;
	std::map<int, Response *> _client_response;

	int _max_fd;

	std::map<int, long> _time;

	in_addr_t _getHostInetAddrFromStr(const std::string& host_str) const;
	long _get_time();
};


#endif //LISTENER_HPP
