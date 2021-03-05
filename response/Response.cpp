//
// Created by jnannie on 2/13/21.
//

#include "Response.hpp"
#include <sstream>
#include <string>
#include <set>
#include "../utils/cpp_libft/libft.hpp"
#include <time.h>

std::set<std::string> Response::implemented_headers = Response::initResponseHeaders();

std::map<int, std::string> Response::status_codes = Response::initStatusCodes();

std::set<std::string> Response::initResponseHeaders() {
	std::set<std::string> implemented_headers;
	implemented_headers.insert("allow"); // Allow: OPTIONS, GET, HEAD
	implemented_headers.insert("content-language"); // Content-Language: en, ase, ru
	implemented_headers.insert("content-length"); // Content-Length: 1348
	implemented_headers.insert("content-location");
	implemented_headers.insert("content-type"); // Content-Type: text/html;charset=utf-8
	implemented_headers.insert("date"); // Date: Tue, 15 Nov 1994 08:12:31 GMT
	implemented_headers.insert("last-modified");
	implemented_headers.insert("location"); // Location: http://example.com/about.html#contacts
	implemented_headers.insert("retry-after");
	implemented_headers.insert("server"); // Server: Apache/2.2.17 (Win32) PHP/5.3.5
	implemented_headers.insert("transfer-encoding"); // Transfer-Encoding: gzip, chunked
	implemented_headers.insert("www-authenticate"); //WWW-Authenticate: Newauth realm="apps", type=1, title="Login to \"apps\"", Basic realm="simple"
	return implemented_headers;
}

std::map<int,std::string> Response::initStatusCodes() {
	std::map<int,std::string> status_codes;
	status_codes[100] = "CONTINUE";
	status_codes[101] = "SWITCHING PROTOCOLS";
	status_codes[200] = "OK";
	status_codes[201] = "CREATED";
	status_codes[202] = "ACCEPTED";
	status_codes[203] = "NON-AUTHORITATIVE INFORMATION";
	status_codes[204] = "NO CONTENT";
	status_codes[205] = "RESET CONTENT";
	status_codes[206] = "PARTIAL CONTENT";
	status_codes[300] = "MULTIPLE CHOICES";
	status_codes[301] = "MOVED PERMANENTLY";
	status_codes[302] = "FOUND";
	status_codes[303] = "SEE OTHER";
	status_codes[304] = "NOT MODIFIED";
	status_codes[305] = "USE PROXY";
	status_codes[307] = "TEMPORARY REDIRECT";
	status_codes[400] = "BAD REQUEST";
	status_codes[401] = "UNAUTHORIZED";
	status_codes[402] = "PAYMENT REQUIRED";
	status_codes[403] = "FORBIDDEN";
	status_codes[404] = "NOT FOUND";
	status_codes[405] = "METHOD NOT ALLOWED";
	status_codes[406] = "NOT ACCEPTABLE";
	status_codes[407] = "PROXY AUTHENTICATION REQUIRED";
	status_codes[408] = "REQUEST TIMEOUT";
	status_codes[409] = "CONFLICT";
	status_codes[410] = "GONE";
	status_codes[411] = "LENGTH REQUIRED";
	status_codes[412] = "PRECONDITION FAILED";
	status_codes[413] = "PAYLOAD TOO LARGE";
	status_codes[414] = "URI TOO LONG";
	status_codes[415] = "UNSUPPORTED MEDIA TYPE";
	status_codes[416] = "RANGE NOT SATISFIABLE";
	status_codes[417] = "EXPECTATION FAILED";
	status_codes[426] = "UPGRADE REQUIRED";
	status_codes[500] = "INTERNAL SERVER ERROR"; // "UPGRADE REQUIRED";
	status_codes[501] = "NOT IMPLEMENTED";
	status_codes[502] = "BAD GATEWAY";
	status_codes[503] = "SERVICE UNAVAILABLE";
	status_codes[504] = "GATEWAY TIMEOUT";
	status_codes[505] = "HTTP VERSION NOT SUPPORTED";
	return status_codes;
}

Response::Response(Request* request, int socket) :
				_request(request), _socket(socket),
				_raw_response(""), _content("") { };

Response::~Response(void) { };


//bool writeFileContentToString(const std::string& file_name, std::string& content) {
//    int file = open(file_name.c_str(), O_RDONLY);
//    if (file == -1)
//        return false;
//
//    // reading file line by line
//    char *str;
//    int ret;
//
//    while ((ret = get_next_line(file, &str)) == 1) {
//        content += str;
//        delete str;
//    }
//    content += str;
//    delete str;
//    close(file);
//    return true;
//}
//
//bool Response::setIndexFileContentToResponseContent(void) {
//    std::string root_path = _request->getAbsoluteRootPathForRequest() + _request->_request_target;
//
//    const std::list<std::string>& index_pages = _request->getIndexPagesListForRequest();
//
//    std::list<std::string>::const_iterator it = index_pages.begin();
//
//    std::string file_content = "";
//    while (it != index_pages.end()) {
//        std::string file_name = root_path + (*it);
//        if (writeFileContentToString(file_name, file_content)) {
//            _content += file_content;
//            return true;
//        }
//        ++it;
//    }
//    return false;
//}

//void Response::generateGetResponse() {
//	std::cout << "TARGET: " << _request->_request_target << std::endl;
//	std::cout << "ABS PATH: " << _request->getAbsoluteRootPathForRequest() << std::endl;
//	setIndexFileContentToResponseContent();
//}

void Response::generateStatusLine() {
	_raw_response += "HTTP/1.1 ";
	_raw_response += libft::ultostr_base(_request->getStatusCode(), 10);
	_raw_response += " ";
	_raw_response += Response::status_codes[_request->getStatusCode()];
	_raw_response += "\r\n";
}

//implemented_headers.insert("allow"); // Allow: OPTIONS, GET, HEAD
//implemented_headers.insert("content-language"); // Content-Language: en, ase, ru
//implemented_headers.insert("content-length"); // Content-Length: 1348
//implemented_headers.insert("content-location");
//implemented_headers.insert("content-type"); // Content-Type: text/html;charset=utf-8
//implemented_headers.insert("date"); // Date: Tue, 15 Nov 1994 08:12:31 GMT
//implemented_headers.insert("last-modified");
//implemented_headers.insert("location"); // Location: http://example.com/about.html#contacts
//implemented_headers.insert("retry-after");
//implemented_headers.insert("server"); // Server: Apache/2.2.17 (Win32) PHP/5.3.5
//implemented_headers.insert("transfer-encoding"); // Transfer-Encoding: gzip, chunked
//implemented_headers.insert("www-authenticate");


/*
 * https://stackoverflow.com/questions/7960318/math-to-convert-seconds-since-1970-into-date-and-vice-versa
 * explanations http://howardhinnant.github.io/date_algorithms.html
 */
struct tm Response::_getCalendarTime(time_t tv_sec) { // TODO: maybe should make it simplier
	struct tm calendar_time;
	int days = tv_sec / 86400;
	days += 719468;
	int era = (days >= 0 ? days : days - 146096) / 146097;
	int doe = days - era * 146097;          // [0, 146096]
	int yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // [0, 399]
	int y = yoe + era * 400;
	int doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // [0, 365]
	int mp = (5 * doy + 2) / 153;                                   // [0, 11]
	int d = doy - (153 * mp + 2) / 5 + 1;                             // [1, 31]
	int m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]

	calendar_time.tm_sec = tv_sec % 60;
	calendar_time.tm_min = tv_sec % 3600 / 60;
	calendar_time.tm_hour = tv_sec % 86400 / 3600;

	calendar_time.tm_mday = d;
	calendar_time.tm_mon = m - 1;
	calendar_time.tm_year = y + (m <= 2) - 1900;

	days = tv_sec / 86400;
	calendar_time.tm_wday = (days >= -4 ? (days + 4) % 7 : (days + 5) % 7 + 6);
//	calendar_time.tm_yday = doy;
//	calendar_time.tm_isdst = 0;
	return calendar_time;
}

struct tm Response::_gmtime(time_t tv_sec) {
	struct tm calendar_time;
	calendar_time = _getCalendarTime(tv_sec);
	return calendar_time;
};

std::string Response::getDateHeader() {
	char s[30]; // Wed, 24 Feb 2021 12:10:04 GMT + '\0'
	struct tm calendar_time;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	calendar_time = _gmtime(tv.tv_sec);
	strftime(s, sizeof(s), "%a, %d %b %Y %H:%M:%S GMT", &calendar_time);

	std::string date_header;
	date_header.append("date: ").append(s).append("\r\n");
	return date_header;
}

std::string Response::getLastModifiedHeader(time_t tv_sec) {
	char s[30]; // Wed, 24 Feb 2021 12:10:04 GMT + '\0'
	struct tm calendar_time;

	calendar_time = _gmtime(tv_sec);
	strftime(s, sizeof(s), "%a, %d %b %Y %H:%M:%S GMT", &calendar_time);

	std::string date_header;
	date_header.append("last-modified: ").append(s).append("\r\n");
	return date_header;
}

std::string Response::getLocationHeader() {
	std::string location;

	location += "location: ";
	location += "http://";
	location += _request->_headers["host"];
//	location += '/';
//	location += "http://localhost:8080"; //TODO:get scheme, host and port from connection or smt
	location += _request->_request_target;//getLocationHeader();
	location += '/';
	location += "\r\n";

	return location;
}

void Response::generateHeaders() {
//	_raw_response += "Content-Type: text/html; charset=utf-8\r\n";
	_raw_response += "server: webserv\r\n";
	_raw_response += getDateHeader();
	_raw_response += _content_type;
	_raw_response += _allow;
	_raw_response += "Content-Length: ";
	_raw_response += libft::ultostr_base(_content.length(), 10);
	_raw_response += "\r\n";
	_raw_response += _last_modified;
	_raw_response += _location;
//	_raw_response += "Connection: keep-alive\r\n"; // TODO: need changes HARDCODE
//	_raw_response += "Content-Language: en\r\n";
	for (std::map<std::string, std::string>::iterator it = _cgi_headers.begin(); it != _cgi_headers.end(); ++it) {
		_raw_response += (*it).first + ": " + (*it).second + "\r\n";
	}
	_raw_response += "\r\n";
}

void Response::generateResponseByStatusCode() {
	_content_type = "Content-Type: text/html\r\n";
	if (_request->_method != "HEAD")
		_content.append(libft::ultostr_base(_request->getStatusCode(), 10)).append(" ").append(Response::status_codes[_request->getStatusCode()]).append("\r\n");

    generateStatusLine();

    if (_request->getStatusCode() != 100){ // cURL dont recognize 100 status code response with headers
        generateHeaders();
		_raw_response.append(_content);
    }
//	_raw_response.append(_content);
//	std::cout << "in Response::generateResponseByStatusCode()\n";
}

void Response::readFileToContent(std::string & filename) {
	char buf[1024 + 1];
	int ret;
	int fd;

	fd = open(filename.c_str(), O_RDONLY);
	while ((ret = read(fd, buf, 1024))) {
		if (ret < 0)
			utils::exitWithLog();
		_content.append(buf, ret);
	}
	close(fd);
}

void Response::generateAutoindex() { // TODO:replace by normal autoindex
	_content = "generated autoindex";
}

void Response::setContentTypeByFileExt(std::string & ext) {
	if (ext == "")
		_content_type = "Content-Type: application/octet-stream\r\n";
	else {
		if (ext == "txt")
			_content_type = "Content-Type: text/plain\r\n";
		else if (ext == "html")
			_content_type = "Content-Type: text/html\r\n";
		else if (ext == "jpg")
			_content_type = "Content-Type: image/jpeg;\r\n";
		else
			_content_type = "Content-Type: application/octet-stream\r\n";
	}
}


/*
 * TODO: case1:
 *         location /html/ {
 *           index index.html;
 *           limit_except PUT;
 *        }
 *        _request->_handling_location is NULL somehow
 *        doesnt work index directive
 *
 * TODO: case2:
 *    location / {
 *    limit_except PUT;
 *    }
 *    doesnt work for files in subfolders
 *
 */
bool Response::isMethodAllowed() {
	if (!_request->_handling_location)
		return true;
	std::list<std::string> allowed_methods = _request->_handling_location->getLimitExceptMethods();

	if (allowed_methods.empty())
		return true;
	for (std::list<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); ++it) {
		if (*it == _request->_method)
			return true;
	}
	return false;
}

void Response::generateGetResponse() {
	generateHeadResponse();

	if (!_request->isStatusCodeOk())
		return ;

	_raw_response += _content;
}

//void Response::setStatusCode(int status_code) {
//	_status_code = status_code;
//}

std::string Response::_getExt(std::string filename) {
	std::string ext;
	size_t dot_pos;

	dot_pos = filename.rfind('.');
	if (dot_pos != std::string::npos)
		ext = filename.substr(dot_pos + 1);
	return ext;
}

bool Response::_isCgiExt(std::string & ext) {
	return ext == "php" || ext == "bla";
}

void Response::_setEnv(char* env[], std::string & filename, std::map<std::string, std::string> & cgiVariables) {
//	std::string temp;
	cgiVariables["AUTH_TYPE"] = "AUTH_TYPE=" + _request->_headers["authorization"];
	env[0] = const_cast<char *>(cgiVariables["AUTH_TYPE"].c_str());
	cgiVariables["CONTENT_LENGTH"] = "CONTENT_LENGTH=" + _request->_headers["content-length"];
	env[1] = const_cast<char *>(cgiVariables["CONTENT_LENGTH"].c_str());
	cgiVariables["CONTENT_TYPE"] = "CONTENT_TYPE=" + _request->_headers["content-type"];
	env[2] = const_cast<char *>(cgiVariables["CONTENT_TYPE"].c_str());
	cgiVariables["GATEWAY_INTERFACE"].assign("GATEWAY_INTERFACE=").append("CGI/1.1");
	env[3] = const_cast<char *>(cgiVariables["GATEWAY_INTERFACE"].c_str());

	/*
	 * TODO: from slack:
	 * TODO: "they want the target with locations substituted" ex :
	 * TODO: PATH_INFO=/YoupieBanane/youpi.bla
	 * TODO: for http://host:port/directory/youpi.bla (in tester)
	 */
	cgiVariables["PATH_INFO"].assign("PATH_INFO=").append(filename);
	env[4] = const_cast<char *>(cgiVariables["PATH_INFO"].c_str());

	cgiVariables["PATH_TRANSLATED"].assign("PATH_TRANSLATED=").append(filename);
	env[5] = const_cast<char *>(cgiVariables["PATH_TRANSLATED"].c_str());
	cgiVariables["QUERY_STRING"] = "QUERY_STRING=" + _request->_query_string;
	env[6] = const_cast<char *>(cgiVariables["QUERY_STRING"].c_str());
	cgiVariables["REMOTE_ADDR"].assign("REMOTE_ADDR=").append("127.0.0.1"); // TODO: need some kind of inet_ntoa()
	env[7] = const_cast<char *>(cgiVariables["REMOTE_ADDR"].c_str());
	cgiVariables["REMOTE_IDENT"] = "REMOTE_IDENT=";
	env[8] = const_cast<char *>(cgiVariables["REMOTE_IDENT"].c_str());
	cgiVariables["REMOTE_USER"] = "REMOTE_USER="; // TODO: https://tools.ietf.org/html/rfc3875#section-4.1.11
	env[9] = const_cast<char *>(cgiVariables["REMOTE_USER"].c_str());
	cgiVariables["REQUEST_METHOD"] = "REQUEST_METHOD=" + _request->_method;
	env[10] = const_cast<char *>(cgiVariables["REQUEST_METHOD"].c_str());
	cgiVariables["REQUEST_URI"].assign("REQUEST_URI=").append("/").append(_request->_request_target); // there is no such Variable in rfc
	env[11] = const_cast<char *>(cgiVariables["REQUEST_URI"].c_str());
	cgiVariables["SCRIPT_NAME"].assign("SCRIPT_NAME=").append("/Users/jnannie/.brew/bin/php-cgi"); // TODO: get from config file
	env[12] = const_cast<char *>(cgiVariables["SCRIPT_NAME"].c_str());
	cgiVariables["SERVER_NAME"] = "SERVER_NAME=" + _request->_headers["host"].substr(0, _request->_headers["host"].find(':'));
	env[13] = const_cast<char *>(cgiVariables["SERVER_NAME"].c_str());
	cgiVariables["SERVER_PORT"] = "SERVER_PORT=" + libft::ultostr_base(_request->_port, 10);
	env[14] = const_cast<char *>(cgiVariables["SERVER_PORT"].c_str());
	cgiVariables["SERVER_PROTOCOL"].assign("SERVER_PROTOCOL=").append("HTTP/1.1");
	env[15] = const_cast<char *>(cgiVariables["SERVER_PROTOCOL"].c_str());
	cgiVariables["SERVER_SOFTWARE"].assign("SERVER_SOFTWARE=").append("webserv");
	env[16] = const_cast<char *>(cgiVariables["SERVER_SOFTWARE"].c_str());
	if (_file_ext == "php") {
		cgiVariables["REDIRECT_STATUS"] = "REDIRECT_STATUS=true";
		env[17] = const_cast<char *>(cgiVariables["REDIRECT_STATUS"].c_str());
	}
	//all elements of env is initialized to NULL
}

void Response::_runCgi(std::string & filename) { // filename is a *.php script
	int pid;
	int exit_status;
	std::string cgi_script;
	if (_file_ext == "php")
		cgi_script = "/Users/jnannie/.brew/bin/php-cgi";
	else
		cgi_script = "/Users/jnannie/Desktop/webserv/cgi_tester";
	char * argv[3] = {
			const_cast<char *>(cgi_script.c_str()),
			const_cast<char *>(filename.c_str()),
			NULL
	};
	char * env[19] = {0};

	std::map<std::string, std::string> cgiVariables;
	_setEnv(env, filename, cgiVariables);

	int filedes_in[2]; // in php-cgi
	int filedes_out[2]; // out of php-cgi
	if (pipe(filedes_in) == -1 || pipe(filedes_out) == -1)
		return _request->setStatusCode(500);

	int stdin_backup = dup(0);
	int stdout_backup = dup(1);
	dup2(filedes_in[1], 1);
	close(filedes_in[1]);
	dup2(filedes_out[0], 0);
	close(filedes_out[0]);
	write(1, _request->_content.c_str(), _request->_content.length());
	if ((pid = fork()) == -1) {
		close(filedes_in[0]);
		close(filedes_out[1]);
		return _request->setStatusCode(500); // Internal Server Error
	} else if (pid == 0) {
		dup2(filedes_in[0], 0);
		dup2(filedes_out[1], 1);
		execve(argv[0], argv, env);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, &exit_status, 0); // TODO: maybe we should use not blocking wait, then we need to save stdin and out backups and pass pipe descritpors to select
	if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	else if (WIFSIGNALED(exit_status))
		exit_status = exit_status | 128;
	if (!exit_status) {
		char buf[1024] = {0};
		fcntl(0, F_SETFL, O_NONBLOCK);
		int ret;
		while ((ret = read(0, buf, 1024)) > 0) {
			try {
				_content.append(buf, ret);
			} catch (std::bad_alloc& ba) {
				_request->setStatusCode(500);
				break ;
			}
		}
	}
	dup2(stdin_backup, 0);
	dup2(stdout_backup, 1);
	if (exit_status)
		_request->setStatusCode(500);
}

void Response::_parseHeadersFromCgiResponse() { // the same as in request headers parsing
	if (!_request->isStatusCodeOk())
		return ;
	std::string field_name;
	std::string field_value;
	size_t field_name_length;
	size_t field_value_length;

	size_t line_length = _content.find("\r\n");
	while (line_length != 0) {
		if (line_length > MAX_HEADER_LINE_LENGTH
			|| line_length == std::string::npos) {
			return _request->setStatusCode(500); // http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
		}

		field_name_length = _content.find(':'); // field-name
		if (field_name_length == std::string::npos) {
			return _request->setStatusCode(500);
		}
		field_name = _content.substr(0, field_name_length);

		libft::string_to_lower(field_name); // field_name is case-insensitive so we make it lowercase to make life easy

		if (field_name.find(' ') != std::string::npos) { // no spaces inside field-name, rfc 2.3.4
			return _request->setStatusCode(500);
		}
		_content.erase(0, field_name_length + 1);


		field_value_length = line_length - field_name_length - 1; // field-value
		if (_content[0] == ' ') {
			_content.erase(0, 1); // remove optional whitespace in the beginning of field-value
			field_value_length--;
		}
		if (_content[field_value_length - 1] == ' ') {
			_content.erase(field_value_length - 1, 1); // remove optional whitespace in the end of field-value
			field_value_length--;
		}
		field_value = _content.substr(0, field_value_length);
		_content.erase(0, field_value_length + 2);

		if (Request::implemented_headers.count(field_name))
		{
			if (_cgi_headers.count(field_name)) {
				if (field_name == "host")
					return _request->setStatusCode(500);
				_cgi_headers[field_name].append(",");
			}
			_cgi_headers[field_name].append(field_value); // add field_name-field_value to map
		}

		line_length = _content.find("\r\n");
	}
	_content.erase(0, 2);
}

void Response::generateHeadResponse() {

//	if (_request->_request_target == "" && _request->_method == "HEAD") { // TODO: remove later, just for test
//		_allow = "Allow: GET";
//		_allow += "\r\n";
//		return _request->setStatusCode(405);
//	}

	if (!isMethodAllowed()) {
		std::list<std::string> allowed_methods = _request->_handling_location->getLimitExceptMethods();
		_allow = "Allow: ";
		for (std::list<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); ++it) {
			_allow += *it;
			_allow += ",";
		}
		_allow.erase(_allow.size() - 1, 1);
		_allow += "\r\n";
		return _request->setStatusCode(405);
	}


//	std::cout << "_request->getAbsoluteRootPathForRequest() " << _request->getAbsoluteRootPathForRequest() << std::endl;
//	std::cout << "_request->_handling_location->getAliasPath() " << _request->_handling_location->getAliasPath() << std::endl;
//	std::cout << "_request->_handling_location->getLocationPath() " << _request->_handling_location->getLocationPath() << std::endl;
//	std::cout << "_request->_handling_location->getRootPath() " << _request->_handling_location->getRootPath() << std::endl;
//	std::cout << "_request->_handling_location->getLocationPathForComparison() " << _request->_handling_location->getLocationPathForComparison() << std::endl;
//	std::cout << "_request->_handling_server->getRootPath() " << _request->_handling_server->getRootPath() << std::endl;


//TODO: need to figure out what path to use instead of root
	std::string filename = _request->getAbsoluteRootPathForRequest();
	if (filename[filename.size() - 1] != '/')
		filename += _request->_request_target; // _request->_request_target always starts with '/'
	else
		filename += _request->_request_target.substr(1); // remove '/'
//	filename = _request->getAbsoluteRootPathForRequest();
//	if (filename[filename.size() - 1] != '/')
//		filename += "/";
//	filename += _request->_request_target.substr(1); // without '/'
//	std::cout << "_root " << _root << "_request->_request_target " << _request->_request_target << std::endl;

	struct stat stat_buf;

	if (stat(filename.c_str(), &stat_buf) == 0) { // file or directory exists
		if (S_ISDIR(stat_buf.st_mode)) { // filename is a directory
			if (filename[filename.size() - 1] != '/') {
//				filename += '/';
				_location = getLocationHeader();

				return _request->setStatusCode(301); //Moved Permanently
			}
			std::list<std::string> index_list;
			if (_request->_handling_location)
				index_list = _request->_handling_location->getIndexPagesDirectiveInfo(); // try to search one of index file
			else
				index_list = _request->_handling_server->getIndexPagesDirectiveInfo(); // try to search one of index file

			if (!index_list.empty()) {
				for (std::list<std::string>::const_iterator it = index_list.begin(); it != index_list.end(); ++it) {
					if (stat((filename + *it).c_str(), &stat_buf) == 0) {
						filename += *it;
						break ;
					}
				}
			}
		}

		if (S_ISREG(stat_buf.st_mode)) {
			_file_ext = _getExt(filename);
			if (_isCgiExt(_file_ext)) {
				_runCgi(filename);
				if (_file_ext == "php") {
					_parseHeadersFromCgiResponse();
					if (_cgi_headers.count("content-length")) {
						_content.resize(libft::strtoul_base(_cgi_headers["content-length"], 10));
					}
				}
//				_content_type = "Content-Type: text/html; charset=UTF-8\r\n";
			} else {
				setContentTypeByFileExt(_file_ext);
				readFileToContent(filename);
				_last_modified = getLastModifiedHeader(stat_buf.st_mtime);
			}
		} else if (S_ISDIR(stat_buf.st_mode)) {
			if (_request->_handling_location && _request->_handling_location->isAutoindexEnabled()) {
				generateAutoindex();
				_content_type = "Content-Type: text/html\r\n";
			} else {
				return _request->setStatusCode(403);
			}
		} else {
			return _request->setStatusCode(403); // TODO:check what code to return if file is not a directory and not a regular file
		}
	} else {
		return _request->setStatusCode(404);
	}

	if (!_request->isStatusCodeOk())
		return ;
//		return generateResponseByStatusCode();

	generateStatusLine();
	generateHeaders();
}

void Response::generatePutResponse() {
    bool file_was_at_start = _request->getFileExistenceStatus();

    if (file_was_at_start) {
        _request->_status_code = 204;
    } else {
        _request->_status_code = 201;
        _location = getLocationHeader();
    }
    generateStatusLine();
    generateHeaders();
    _raw_response += _content;
}

void Response::generatePostResponse() {

//	if (_request->_request_target == "") { // TODO: remove later, just for test
//		_allow = "Allow: GET";
//		_allow += "\r\n";
//		return _request->setStatusCode(405);
//	}

	if (!isMethodAllowed()) {
		std::list<std::string> allowed_methods = _request->_handling_location->getLimitExceptMethods();
		_allow = "Allow: ";
		for (std::list<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); ++it) {
			_allow += *it;
			_allow += ",";
		}
		_allow.erase(_allow.size() - 1, 1);
		_allow += "\r\n";
		return _request->setStatusCode(405);
	}


//	struct stat stat_buf;

//TODO: need to figure out what path to use instead of root
	std::string filename = _request->getAbsoluteRootPathForRequest();
	if (filename[filename.size() - 1] != '/')
		filename += _request->_request_target; // _request->_request_target always starts with '/'
	else
		filename += _request->_request_target.substr(1); // remove '/'

	_file_ext = _getExt(filename);
	if (_isCgiExt(_file_ext)) {
		_runCgi(filename);
		if (_file_ext == "php" || _file_ext == "bla") {
			_parseHeadersFromCgiResponse();
			if (_cgi_headers.count("content-length")) {
				_content.resize(libft::strtoul_base(_cgi_headers["content-length"], 10));
			}
		}
	}

	if (!_request->isStatusCodeOk())
		return ;

	generateStatusLine();
	generateHeaders();
	_raw_response += _content;
}

void Response::generateResponse() {
	if (_request->isStatusCodeOk()) {
		if (_request->_method == "GET") {
			generateGetResponse();
		} else if (_request->_method == "HEAD") {
			generateHeadResponse();
		} else if (_request->_method == "PUT") {
			generatePutResponse();
		} else if (_request->_method == "POST") {
			generatePostResponse();
		} else {
			_request->setStatusCode(501); // 501 Not Implemented
		}
	}
	if (!_request->isStatusCodeOk()) {
		generateResponseByStatusCode();
	}
}

void Response::sendResponse() {
	// Отправляем ответ клиенту с помощью функции send
    std::cout << _raw_response << std::endl;
    send(_socket, _raw_response.c_str(), _raw_response.length(), 0);
	//	std::cout << "Response::sendResponse response is sent\n";
}


/*
PUT /nginx_meme.jpg HTTP/1.1

Host: localhost:8080

User-Agent: curl/7.75.0

Accept: *//*

Content-Length: 172172

Expect: 100-continue






 */