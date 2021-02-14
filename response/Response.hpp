//
// Created by jnannie on 2/12/21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../request_handlers/Request.hpp"
#include <sys/socket.h>
#include <string>

class Response {

public:
	Response(Request* request, int socket);

	~Response(void);

	void sendResponse();

	static std::map<int,std::string> initStatusCodes();

public:
	static const std::map<int, std::string> status_codes;

private:
	std::string _raw_response;
	Request* _request;
	int _socket;

};

#endif //RESPONSE_HPP
