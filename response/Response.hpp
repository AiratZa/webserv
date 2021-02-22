//
// Created by jnannie on 2/12/21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../request_handlers/Request.hpp"
#include <sys/socket.h>
#include <string>
#include <set>
#include <list>

class Response {
public:
	Response(Request* request, int socket);

	~Response(void);

	void sendResponse();

	void generateGetResponse();
	void generateHeadResponse();
	void generatePutResponse();
	void generateResponse();

private:
	static std::map<int,std::string> initStatusCodes();
	static std::set<std::string> initResponseHeaders();

public:
	static const std::set<std::string> implemented_headers;
	static const std::map<int, std::string> status_codes;

private:
	Request* _request;
	int _socket;
	std::string _http_version;
//	int _status_code;
	std::string _reason_phrase;
	std::map<std::string, std::string> _headers;
	std::string _raw_response;
	std::string _content;

};

	std::string    parsURL(std::string url);
#endif //RESPONSE_HPP
