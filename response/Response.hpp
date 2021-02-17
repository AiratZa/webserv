//
// Created by jnannie on 2/12/21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../request_handlers/Request.hpp"
#include <sys/socket.h>
#include <string>
#include <set>

class Response {
public:
	Response(Request* request, int socket);

	~Response(void);

	void sendResponse();

private:
	static std::map<int,std::string> initStatusCodes();
	static std::set<std::string> initResponseHeaders();

public:
	static const std::set<std::string> implemented_headers;
	static const std::map<int, std::string> status_codes;

private:
	std::string _raw_response;
	Request* _request;
	int _socket;

};

#endif //RESPONSE_HPP
