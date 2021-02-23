#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include <set>

#include "../config_parsing/Config.hpp"
#include <climits>

class Request {

	public:
		Request();
		Request(const std::string& request);
		~Request(void);

		std::string & getRawRequest(void);
		void setRawRequest(const std::string & request);


		void setStatusCode(int status_code);
		void parseRequestLine();
		void stringToLower(std::string & str);
		bool isStatusCodeOk();
		void parseChunkedContent();
		void getContentByLength();
		void parseHeaders();
		void parseBody();
//		void checkMethod();
//		void checkRequestTarget();
//		void checkHttpVersion();
//		void checkHeaders();

	private:
		static std::set<std::string> initRequestHeaders();

	private:
		std::string _raw_request;

	public:
		static const std::set<std::string> implemented_headers;

	public:
		int _status_code;
		std::string _method;
		std::string _request_target;
		std::string _http_version;

		std::map<std::string, std::string> _headers;

		std::string _content;

		size_t _client_max_body_size; // need to use client_max_body_size from server config

    void setHandlingServer(ServerContext* handling_server); // Airat
    void setHandlingLocation(LocationContext* location_to_route); // Airat

	std::string    parsURL(std::string url);

    public:
        ServerContext* _handling_server; // Airat
        LocationContext* _handling_location; // Airat
//		bool _is_chunked;
//		bool _is_content_lenght;
//		size_t _content_lenght;

};

#endif
