#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <map>
#include <set>

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
		void parse();

private:
	static std::set<std::string> initRequestHeaders();

	private:
		std::string _raw_request;

	public:
		int _status_code;
		std::string _method;
		std::string _request_target;
		std::string _http_version;

		std::map<std::string, std::string> _headers;

		std::string _content;

		static const std::set<std::string> implemented_headers;

		unsigned long _client_max_body_size;



//		bool _is_chunked;
//		bool _is_content_lenght;
//		size_t _content_lenght;

};

#endif
