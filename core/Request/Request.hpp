#ifndef REQUEST_HPP
# define REQUEST_HPP

class Request;
#include <map>
#include <set>
#include <string>
#include <climits>

#include "../WebServ.hpp"
#include "../Response/Response.hpp"
#include "../../config_parsing/Config.hpp"

#define MAX_HEADER_LINE_LENGTH 8192 //http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
#define DEFAULT_REQUEST_STATUS_CODE 200

#define BUFFER_LENGHT 1024
#define TIME_OUT 300000


class Request {

	friend class Response;
	friend class Listener;
	friend class WebServ;

public:
	static std::set<std::string>    initRequestHeaders();
	static std::list<int>           initOkStatusCodes(void);

	static const std::set<std::string>  implemented_headers;
	static const std::list<int>         OK_STATUS_CODES;

private:
	int                             _status_code;
	std::map<std::string, std::string>  _headers;
	std::string                         _raw_request;
	struct sockaddr_in                  _remote_addr;
	std::string                         _method;
	std::string                         _request_target;
	std::string                         _query_string;
	std::string                         _http_version;
	std::string                         _content;
	int                                 _server_port;
	bool                                _close_connection;
	ServerContext*                      _handling_server;
	LocationContext*                    _handling_location;
	std::string                         _absolute_root_path_for_request;
	bool                                _is_alias_path;
	char                                _buf[BUFFER_LENGHT];
	long                                 _bytes_read;
	std::string                         _full_filename;

	std::size_t         _header_end_pos;
	bool                _header_was_read;
	bool                _is_file_exists;
	long long           _only_content_length_read_body_size;
	bool                _is_need_writing_body_to_file;
	std::string         _cgi_script_path;
	std::string         _response_content_lang;

	bool            _is_chunked;
	std::string     _host;
	int             _port;
	std::size_t     _is_lang_file_pos;

public:
	Request();
	Request(struct sockaddr_in & remote_addr, int server_port);
	~Request(void);

	void setStatusCode(int status_code);
	void setStatusCodeNoExept(int status_code);
	void setHandlingServer(ServerContext* handling_server);
	void setHandlingLocation(LocationContext* location_to_route);
	void setAbsoluteRootPathForRequest(void);
	void setHeaderWasRead(void);
	void setHeaderEndPos(std::size_t val);
	void setFileExistenceStatus(bool value);
	void setNeedWritingBodyToFile(bool value);
	void setCgiScriptPathForRequest(const std::string& path);
	void setHostAndPort(const std::string& host, const int port);
	void setReponseContentLang(const std::string& lang);

	std::string &           getRawRequest(void);
	std::string             getAbsolutePathForPUTRequests(void) const;
	const std::string&      getAbsoluteRootPathForRequest(void) const;
	int                     getStatusCode();
	long long               getOnlyContentLengthReadBodySize(void);
	bool                    getFileExistenceStatus(void) const;
	bool                    getNeedWritingBodyToFile(void) const;
	const std::string&      getReponseContentLang(void);
	const std::string&      getCgiScriptPathForRequest(void) const;

	void parseRequestLine(void);
	void parsURL();
	void parseHeaders(void);
	void handleExpectHeader(void);
	void handleAcceptCharsetHeader(void);
	void handleAcceptLanguageHeader(bool is_header_exists);
	void increaseOnlyContentLengthReadBodySize(long bytes_read);
	bool isHeaderWasRead(void) const;
	bool isStatusCodeOk();
	bool checkToClientMaxBodySize(void);
	bool checkToClientMaxBodySize(long long int value_to_check);
	bool writeBodyReadBytesIntoFile();
	bool checkIsMayFileBeOpenedOrCreated(void);
	bool isFileExists(void);
	bool isFileExists(const std::string& full_filename);
	bool isRegFileExists(const std::string& full_filename);
	bool isConcreteHeaderExists(const std::string& header_name);
	bool targetIsFile(void);
	void appendRequestTarget(std::string & filename);
	bool isMethodLimited(const LocationContext& handling_location);
};



#endif
