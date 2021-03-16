//
// Created by jnannie on 2/12/21.
//

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

class Response;

#include "../Request/Request.hpp"
#include <sys/socket.h>
#include <string>
#include <set>
#include <list>
//#include <sys/types.h>
//#include <sys/wait.h>


class Response {
public:
	Response(Request * request, int socket);
	Response();
	~Response(void);

	void generateResponse();
	void setRemains();
	bool getInProgress();
	std::string & getContent();
	void sendResponse();

private:
	void _generateGetResponse();
	void _generateHeadResponseCore();
	void _generateHeadResponse();
	void _generatePutResponse();
	void _generatePostResponse();
	void _generateResponseByStatusCode();
	void _generateStatusLine();
	void _generateHeaders();
//	void _generateContent();
	void _readFileToContent(std::string & filename);
	std::string _generateAutoindex(std::string dir_name);
	void _setContentTypeByFileExt(std::string & ext);
	bool _isMethodAllowed();
//	bool _setIndexFileContentToResponseContent(void); // Airat
	void _updateRequestForErrorPage(const std::string& error_page_link);
	void _generateDefaultResponseByStatusCode();
	void _generateResponseForErrorPage(void);
	const std::string _searchForErrorPageLinkAndSetChangeError() const;
	void _checkForAcceptPrefixHeaders(void);
	std::string _getDateHeader();
	std::string _getLastModifiedHeader(time_t tv_sec);
	std::string _getLocationHeader(bool is_file);
	std::string _getAllowHeader();
	std::string _getWwwAuthenticateHeader();
	std::string _getRetryAfterHeader();
	std::string _getUserFromCredentials();
	struct tm _getCalendarTime(time_t tv_sec);
	struct tm _gmtime(time_t tv_sec);
	std::string _getExt(std::string filename);
	bool _isCgiExt();
	void _runCgi(std::string & filename);
	void _setEnv(std::vector<char *> & env, std::string & filename, std::map<std::string, std::string> & cgiVariables);
	void _parseHeadersFromCgiResponse();
	std::string _inet_ntoa(struct in_addr sin_addr);
	static std::map<int,std::string> _initStatusCodes();
	static std::set<std::string> _initResponseHeaders();
	std::list<std::map<std::string, std::list<std::string> > > _dir_opers(const std::string& dir_name);
	std::string _replaceQuoteToCode(const std::string& str);
	std::size_t _getCharsLen(const std::string& str);


public:
	static std::set<std::string> implemented_headers;
	static std::map<int, std::string> status_codes;

private:
	Request * _request;
	int _socket;
	std::string _http_version;
	std::map<std::string, std::string> _headers;
	std::string _raw_response;
// headers
	std::string _content_type;
	std::string _allow;
	std::string _last_modified;
	std::string _location;
	std::string _content_location;
	std::string _www_authenticate;
	std::string _retry_after;
	std::string _content;
	std::string _file_ext;
	std::string _cgi_response;
//	std::string _cgi_status_line;
	std::map<std::string, std::string> _cgi_headers;
	bool _in_progress;
	long _remains;
	long _sent_len;
	int _error_code_for_generaion;

};



#endif //RESPONSE_HPP
