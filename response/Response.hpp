//
// Created by jnannie on 2/12/21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../request_handlers/Request.hpp"
#include <sys/socket.h>
#include <string>
#include <set>
//#include "../Server.hpp"

//class Server;
#include <list>
#include <sys/stat.h>
#include <fcntl.h>

class Response {
public:
	Response(Request* request, int socket);

	~Response(void);

	void sendResponse();

	void generateGetResponse();
	void generateHeadResponse();
	void generatePutResponse();
	void generateResponse();
	void generateResponseByStatusCode();
	void generateStatusLine();
	void generateHeaders();
	void generateContent();
	void readFileToContent(std::string & filename);
	bool isStatusCodeOk();
	void generateAutoindex();
	void setContentTypeByFilename(std::string & filename);



private:
	static std::map<int,std::string> initStatusCodes();
	static std::set<std::string> initResponseHeaders();

public:
	static std::set<std::string> implemented_headers;
	static std::map<int, std::string> status_codes;

private:
	Request* _request;
	int _socket;
	std::string _http_version;
	int _status_code;
	std::string _reason_phrase;
	std::map<std::string, std::string> _headers;
	std::string _raw_response;
	std::string _content;
//	Server* _server;

	std::string _root; // TODO:temporary

private: // headers
	std::string _content_type;

    bool setIndexFileContentToResponseContent(void); // Airat

};


#endif //RESPONSE_HPP
