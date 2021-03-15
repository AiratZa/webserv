//
// Created by jnannie on 2/13/21.
//

#include <time.h>

#include <sstream>
#include <string>
#include <set>

#include "Response.hpp"
#include "../../utils/cpp_libft/libft.hpp"
#include "../../base64_coding/base64.hpp"
#include "autoindex_handling/autoindex_handling.hpp"


extern bool g_sigpipe;

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
	status_codes[401] = "UNAUTHORIZED"; // Authorization Required //UNAUTHORIZED
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

Response::Response() :
		_request(), _socket(),
		_raw_response(""), _content(""),
		in_progress(false), sent_len(0), error_code_for_generaion(200) { };

Response::Response(Request * request, int socket) :
				_request(request), _socket(socket),
				_raw_response(""), _content(""),
				in_progress(false), sent_len(0), error_code_for_generaion(200) { };

Response::~Response(void) { };


void Response::generateStatusLine() {
	_raw_response += "HTTP/1.1 ";
	_raw_response += libft::ultostr_base(_request->getStatusCode(), 10);
	_raw_response += " ";
	_raw_response += Response::status_codes[_request->getStatusCode()];
	_raw_response += "\r\n";
}

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

std::string Response::getLocationHeader(bool is_file) {
	std::string location;

	location += "location: ";
	location += "http://";
	location += _request->_headers["host"];
	location += _request->_request_target;//getLocationHeader();
	if (!is_file) {
        location += '/';
	}
	location += "\r\n";

	return location;
}

std::string Response::getAllowHeader() {
	std::list<std::string> allowed_methods = _request->_handling_location->getLimitExceptMethods();
	std::string allow = "Allow: ";
	for (std::list<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); ++it) {
		allow += *it;
		allow += ",";
	}
	allow.erase(allow.size() - 1, 1);
	allow += "\r\n";
	return allow;
}

std::string Response::getWwwAuthenticateHeader() {
	std::string www_authenticate = "WWW-Authenticate: Basic realm=\"restricted\", charset=\"UTF-8\"\r\n";
	return www_authenticate;
}

std::string Response::getRetryAfterHeader() {
    std::string retryAfter = ("Retry-After: " + std::string(RETRY_AFTER_SECOND_DELAY) + "\r\n");
    return retryAfter;

}

void Response::generateHeaders() {
	_raw_response += "server: webserv\r\n";
	_raw_response += _www_authenticate;
	_raw_response += getDateHeader();
	_raw_response += _content_type;
	_raw_response += _allow;
	_raw_response += _retry_after;
	_raw_response += "Content-Length: ";
	_raw_response += libft::ultostr_base(_content.length(), 10);
	_raw_response += "\r\n";
	_raw_response += _last_modified;
	_raw_response += _location;
	_raw_response += _content_location;

    if (_request->_is_lang_file_pos)
        _raw_response += "Content-Language: " "en-US, ru-RU" "\r\n";

    for (std::map<std::string, std::string>::iterator it = _cgi_headers.begin(); it != _cgi_headers.end(); ++it) {
		_raw_response += (*it).first + ": " + (*it).second + "\r\n";
	}
	_raw_response += "\r\n";
}

void Response::updateRequestForErrorPage(const std::string& error_page_link) {
    error_code_for_generaion = _request->getStatusCode();

    _request->setStatusCode(200);
    if (_request->_method != "HEAD") {
        _request->_method = "GET";
    }
    _request->_request_target = error_page_link;
    WebServ::routeRequest(_request->_host, _request->_port, _request, _request->_request_target);

}

void Response::generateResponseForErrorPage(void) {


    generateHeadResponseCore();

    // check for the same error
    if (_request->getStatusCode() == error_code_for_generaion) {
        return generateDefaultResponseByStatusCode();
    }
    _request->setStatusCodeNoExept(error_code_for_generaion);
    error_code_for_generaion = 200;

    generateStatusLine();

	if (_request->getStatusCode() == 401) {
		_www_authenticate = getWwwAuthenticateHeader();
	}

    generateHeaders();
    if (_request->_method != "PUT") {
        _raw_response += _content;
    }
}

const std::string Response::searchForErrorPageLinkAndSetChangeError(void) const {
    AContext * context = (_request->_handling_location != NULL)?
                         static_cast<AContext*>(_request->_handling_location) :
                         static_cast<AContext*> (_request->_handling_server);

    if (context)
    {
        const std::map<int, std::map<std::string, std::string> > &error_page_info = context->getErrorPagesDirectiveInfo();

        std::map<int, std::map<std::string, std::string> >::const_iterator it_search = error_page_info.find(
                _request->getStatusCode());

        if (it_search != error_page_info.end()) {
            const std::map<std::string, std::string> &params_map = (it_search->second);

            std::string change_error_code = "";
            std::string redirect_uri = "";

            std::map<std::string, std::string>::const_iterator param_it;

            param_it = params_map.find(ERROR_PAGE_CHANGE_ERROR_CODE);
            if (param_it != params_map.end()) {
                change_error_code = param_it->second;
                if (change_error_code.size()) {
                    _request->setStatusCode(libft::stoll_base(change_error_code, 10));
                }
            }

            param_it = params_map.find(ERROR_PAGE_REDIRECT_URI);
            if (param_it != params_map.end()) {
                redirect_uri = param_it->second;
            }
            return redirect_uri;
        }
    }
    return "";
}


void Response::generateDefaultResponseByStatusCode() {
    _content_type = "Content-Type: text/html;charset=utf-8\r\n";
    if (_request->_method != "HEAD")
        _content.append(libft::ultostr_base(_request->getStatusCode(), 10)).append(" ").append(Response::status_codes[_request->getStatusCode()]).append("\r\n");

    generateStatusLine();

    if (_request->getStatusCode() == 401) {
        _www_authenticate = getWwwAuthenticateHeader();
    }

    if (_request->getStatusCode() != 100){ // cURL dont recognize 100 status code response with headers
        generateHeaders();
        _raw_response.append(_content);
    }

}

void Response::generateResponseByStatusCode() {

    std::string link = searchForErrorPageLinkAndSetChangeError();
    _content = "";
	if (link.size()) {
        updateRequestForErrorPage(link);
        generateResponseForErrorPage();
    }
	else
    {
        generateDefaultResponseByStatusCode();
    }

}

void Response::readFileToContent(std::string & filename) {
	char buf[1024 + 1];
	int ret;
	int fd;

	fd = open(filename.c_str(), O_RDONLY);
    if (fd <= 0) {
        _request->setStatusCode(500);
        return ;
    }

	while ((ret = read(fd, buf, 1024))) {
		if (ret < 0)
			utils::exitWithLog();
		_content.append(buf, ret);
	}
	close(fd);
}

void Response::generateAutoindex(std::string & filename) { // TODO:replace by normal autoindex
	_content = write_html(filename, _request);
}

void Response::setContentTypeByFileExt(std::string & ext) {
	if (ext == "")
		_content_type = "Content-Type: application/octet-stream\r\n";
	else {
		if (ext == "txt")
			_content_type = "Content-Type: text/plain;charset=utf-8\r\n";
		else if (ext == "html")
			_content_type = "Content-Type: text/html;charset=utf-8\r\n";
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

std::string Response::_getExt(std::string filename) {
	std::string ext;
	size_t dot_pos;

	dot_pos = filename.rfind('.');
	if (dot_pos != std::string::npos)
		ext = filename.substr(dot_pos + 1);
	return ext;
}

bool Response::_isCgiExt() {
	if (_request->getCgiScriptPathForRequest().empty())
		return false;
	return true;
}

/*
 * https://topic.alibabacloud.com/a/implement-inet_aton-and-inet_ntoa-function-functions-in-c-language_1_31_20001161.html
 */
std::string Response::_inet_ntoa(struct in_addr sin_addr) {
	std::string buf;
	unsigned char *str = (unsigned char *)&sin_addr;

	buf += libft::ultostr_base(static_cast<unsigned long>(str[0]), 10) + ".";
	buf += libft::ultostr_base(static_cast<unsigned long>(str[1]), 10) + ".";
	buf += libft::ultostr_base(static_cast<unsigned long>(str[2]), 10) + ".";
	buf += libft::ultostr_base(static_cast<unsigned long>(str[3]), 10);

	return buf;
}

std::string Response::_getUserFromCredentials() {
	std::string user;
	size_t credential_pos = _request->_headers["authorization"].find(" ");
	credential_pos++;
	if (credential_pos != std::string::npos && _request->_headers["authorization"].size() > credential_pos) {
		std::string credentials = _request->_headers["authorization"].substr(credential_pos);
		credentials = credentials.substr(credentials.find_first_not_of(" "));
		user = Base64::base64_decode(credentials).substr(0, credentials.find(':'));
	}
	return user;
}


void Response::_setEnv(std::vector<char *> & env, std::string & filename, std::map<std::string, std::string> & cgiVariables) {
	cgiVariables["AUTH_TYPE"] = "AUTH_TYPE=" + _request->_headers["authorization"].substr(0, _request->_headers["authorization"].find(' '));
	cgiVariables["CONTENT_LENGTH"] = "CONTENT_LENGTH=" + _request->_headers["content-length"];
	cgiVariables["CONTENT_TYPE"] = "CONTENT_TYPE=" + _request->_headers["content-type"];
	cgiVariables["GATEWAY_INTERFACE"].assign("GATEWAY_INTERFACE=").append("CGI/1.1");

	cgiVariables["PATH_INFO"].assign("PATH_INFO=").append(_request->_request_target);

	cgiVariables["PATH_TRANSLATED"].assign("PATH_TRANSLATED=").append(filename);
	cgiVariables["QUERY_STRING"] = "QUERY_STRING=" + _request->_query_string;
	cgiVariables["REMOTE_ADDR"].assign("REMOTE_ADDR=").append(_inet_ntoa(_request->_remote_addr.sin_addr).c_str());
	cgiVariables["REMOTE_IDENT"] = "REMOTE_IDENT=";

	cgiVariables["REMOTE_USER"].assign("REMOTE_USER=").append(_getUserFromCredentials());
	cgiVariables["REQUEST_METHOD"] = "REQUEST_METHOD=" + _request->_method;
	cgiVariables["REQUEST_URI"].assign("REQUEST_URI=").append(_request->_request_target); // there is no such Variable in rfc
	cgiVariables["SCRIPT_NAME"].assign("SCRIPT_NAME=").append(_request->_handling_location->getCgiScript().c_str());
	cgiVariables["SERVER_NAME"] = "SERVER_NAME=" + _request->_headers["host"].substr(0, _request->_headers["host"].find(':'));
	cgiVariables["SERVER_PORT"] = "SERVER_PORT=" + libft::ultostr_base(_request->_server_port, 10);
	cgiVariables["SERVER_PROTOCOL"].assign("SERVER_PROTOCOL=").append("HTTP/1.1");
	cgiVariables["SERVER_SOFTWARE"].assign("SERVER_SOFTWARE=").append("webserv");

	for (std::map<std::string, std::string>::iterator it = _request->_headers.begin(); it != _request->_headers.end(); ++it) {
		if (it->first != "content-length" && it->first != "content-Type" && it->first != "authorization") {
			std::string first = it->first;
			std::replace(first.begin(), first.end(), '-', '_');
			libft::string_to_upper(first);
			cgiVariables[first] = "HTTP_" + first + "=" + it->second;
		}
	}

	if (_file_ext == "php") {
		cgiVariables["REDIRECT_STATUS"] = "REDIRECT_STATUS=true"; // php doesnt work without it, dont know why yet
	}
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = cgiVariables.begin(); it != cgiVariables.end(); ++it) {
		env.push_back(const_cast<char *>(it->second.c_str()));
		i++;
	}
	env.push_back(NULL);
	//all elements of env are initialized to NULL
}



void Response::_runCgi(std::string & filename) { // filename is a *.php script
	int pid;
    	int exit_status = 0;
	std::string cgi_script;
	cgi_script = _request->getCgiScriptPathForRequest();
	char * argv[3] = {
			const_cast<char *>(cgi_script.c_str()),
			const_cast<char *>(filename.c_str()),
			NULL
	};
	std::vector<char *> env;

	std::map<std::string, std::string> cgiVariables;
	_setEnv(env, filename, cgiVariables);

	std::string out_file_path = WebServ::getWebServRootPath() + "temp_out";
	std::string in_file_path = WebServ::getWebServRootPath() + "temp_in";

	int fd_write;
	if ((fd_write = open(in_file_path.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRWXU)) == -1)
		utils::exitWithLog();

	long ret;
	ret = write(fd_write, _request->_content.c_str(), _request->_content.size());
	if (ret < 0) {
		return _request->setStatusCode(500);
	}

	close(fd_write);

	if ((pid = fork()) == -1) {
		 utils::exitWithLog();
	} else if (pid == 0) {
		if ((fd_write = open(in_file_path.c_str(), O_RDONLY, S_IRWXU)) == -1)
			utils::exitWithLog();
		dup2(fd_write, 0);
		close(fd_write);
		int fd_read;
		if ((fd_read = open(out_file_path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU)) == -1)
			utils::exitWithLog();
		dup2(fd_read, 1);
		close(fd_read);
		execve(argv[0], argv, &env[0]);
		exit(EXIT_FAILURE);
	}

	waitpid(pid, &exit_status, 0); // TODO: maybe we should use not blocking wait, then we need to save stdin and out backups and pass pipe descritpors to select
	if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	else if (WIFSIGNALED(exit_status))
		exit_status = exit_status | 128;

	if (exit_status == 0) {

	struct stat stat_buf;
	size_t size = 0;

	if (stat(out_file_path.c_str(), &stat_buf) == 0) { // getting file size of "out_file_path"
		size = stat_buf.st_size;
	} else {
		utils::exitWithLog();
	}

    int fd_read;

    if ((fd_read = open(out_file_path.c_str(), O_RDONLY, S_IRWXU)) == -1) {
        utils::exitWithLog();
    }

	std::vector<char> buf;
	buf.reserve(size);
    ret = 0;
    if ((ret = read(fd_read, &buf[0], size)) >= 0) {
			try {
				_cgi_response.append(&buf[0], ret);
				size_t headers_end = _cgi_response.find("\r\n\r\n") + 4;
				if (headers_end != std::string::npos) {
					std::string headers = _cgi_response.substr(0, headers_end);
					if (headers.find("content-length") != std::string::npos) {
					}
				}
			} catch (std::bad_alloc& ba) {
				_request->setStatusCodeNoExept(500);
			}
		}
    	else {
			_request->setStatusCodeNoExept(500);
    	}
		close(fd_read);
	}

	unlink(in_file_path.c_str());
	unlink(out_file_path.c_str());

	if (exit_status || !_request->isStatusCodeOk())
		_request->setStatusCode(500);
}

void Response::_parseHeadersFromCgiResponse() { // the same as in request headers parsing
	if (!_request->isStatusCodeOk())
		return ;
	std::string field_name;
	std::string field_value;
	size_t field_name_length;
	size_t field_value_length;

	long headers_len = _cgi_response.find("\r\n\r\n") + 4;
	std::string headers = _cgi_response.substr(0, headers_len);

	size_t line_length = headers.find("\r\n");
	while (line_length != 0) {
		if (line_length > MAX_HEADER_LINE_LENGTH
			|| line_length == std::string::npos) {
			return _request->setStatusCode(500); // http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
		}

		field_name_length = headers.find(':'); // field-name
		if (field_name_length == std::string::npos) {
			return _request->setStatusCode(500);
		}
		field_name = headers.substr(0, field_name_length);

		libft::string_to_lower(field_name); // field_name is case-insensitive so we make it lowercase to make life easy

		if (field_name.find(' ') != std::string::npos) { // no spaces inside field-name, rfc 2.3.4
			return _request->setStatusCode(500);
		}
		headers.erase(0, field_name_length + 1);


		field_value_length = line_length - field_name_length - 1; // field-value
		if (headers[0] == ' ') {
			headers.erase(0, 1); // remove optional whitespace in the beginning of field-value
			field_value_length--;
		}
		if (headers[field_value_length - 1] == ' ') {
			headers.erase(field_value_length - 1, 1); // remove optional whitespace in the end of field-value
			field_value_length--;
		}
		field_value = headers.substr(0, field_value_length);
		headers.erase(0, field_value_length + 2);

		_cgi_headers[field_name].append(field_value);

		line_length = headers.find("\r\n");
	}
	_cgi_response.erase(0, headers_len);
}


// like pure func
void _appendRequestTarget(std::string & filename, Request* _request) {
    if (_request->_handling_location) {
        std::string request_substr = _request->_request_target.substr(_request->_handling_location->getLocationPath().length());
        if (filename[filename.size() - 1] != '/') {
            if (request_substr[0] != '/')
                filename += '/';
            filename += request_substr; // _request->_request_target always starts with '/'
        } else {
            if (request_substr[0] == '/')
                filename += request_substr.substr(1); // remove '/'
            else
                filename += request_substr;
        }
    } else {
        if (filename[filename.size() - 1] != '/')
            filename += _request->_request_target; // _request->_request_target always starts with '/'
        else
            filename += _request->_request_target.substr(1); // remove '/'
    }
}


// for existence check ONLY
void _appendRequestTarget(std::string & filename, Request* _request, const std::string& tmp_request_target) {
    if (_request->_handling_location) {
        std::string request_substr = tmp_request_target.substr(_request->_handling_location->getLocationPath().length());
        if (filename[filename.size() - 1] != '/') {
            if (request_substr[0] != '/')
                filename += '/';
            filename += request_substr; // _request->_request_target always starts with '/'
        } else {
            if (request_substr[0] == '/')
                filename += request_substr.substr(1); // remove '/'
            else
                filename += request_substr;
        }
    } else {
        if (filename[filename.size() - 1] != '/')
            filename += tmp_request_target; // _request->_request_target always starts with '/'
        else
            filename += tmp_request_target.substr(1); // remove '/'
    }
}

void Response::generateHeadResponseCore() {

    if (!isMethodAllowed() && (_request->getCgiScriptPathForRequest()).empty()) {
        _allow = getAllowHeader();
        return _request->setStatusCode(405);
    }

//TODO: need to figure out what path to use instead of root
    std::string filename = _request->getAbsoluteRootPathForRequest();
	_appendRequestTarget(filename, _request);

    struct stat stat_buf;
    std::string matching_index;

    if (stat(filename.c_str(), &stat_buf) == 0) { // file or directory exists
        if (S_ISDIR(stat_buf.st_mode)) { // filename is a directory
            if (filename[filename.size() - 1] != '/') {
                _location = getLocationHeader(false);
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
						matching_index = *it;
                        filename += *it;
                        WebServ::routeRequest(_request->_host, _request->_port, _request, _request->_request_target + *it);
                        break ;
                    }
                }
                if (matching_index.empty() && !_request->_handling_server->isAutoindexEnabled() && (!_request->_handling_location || (_request->_handling_location && !_request->_handling_location->isAutoindexEnabled()))) { // test from subject wants 404 if there is index in config but file doesnt exist
                    return _request->setStatusCode(404);
                }
            }
        }

		if (S_ISREG(stat_buf.st_mode)) {
			_file_ext = _getExt(filename);
			if (_isCgiExt()) {
				_runCgi(filename);
				_parseHeadersFromCgiResponse();
				if (_cgi_headers.count("content-length")) {
					_cgi_response.resize(libft::strtoul_base(_cgi_headers["content-length"], 10));
				}
				_content.swap(_cgi_response);
			} else {
				setContentTypeByFileExt(_file_ext);
				readFileToContent(filename);
				_last_modified = getLastModifiedHeader(stat_buf.st_mtime);
			}
		} else if (S_ISDIR(stat_buf.st_mode)) {
			if (filename[filename.size() - 1] != '/') {
				if (_request->_request_target[_request->_request_target.size() - 1] != '/')
					_request->_request_target += '/';
				_request->_request_target += matching_index;

				_location = getLocationHeader(false); // TODO: need checks. ,aybe it is file location

                _retry_after = getRetryAfterHeader();
                return _request->setStatusCode(301); //Moved Permanently
            }
            if ((_request->_handling_location && _request->_handling_location->isAutoindexEnabled()) || _request->_handling_server->isAutoindexEnabled()) {
                generateAutoindex(filename);
                _content_type = "Content-Type: text/html;charset=utf-8\r\n";
            } else {
                return _request->setStatusCode(403);
            }
        } else {
            return _request->setStatusCode(403); // TODO:check what code to return if file is not a directory and not a regular file
        }
    } else {
        return _request->setStatusCode(404);
    }
}

void Response::generateHeadResponse() {

    generateHeadResponseCore();
	if (!_request->isStatusCodeOk())
		return ;



	generateStatusLine();
	generateHeaders();
}

void Response::generatePutResponse() {
    bool file_was_at_start = _request->getFileExistenceStatus();

    if (file_was_at_start) {
        _request->setStatusCode(204);
    } else {
        _request->setStatusCode(201);
        _content_location = "Content-Location: " + _request->_request_target + "\r\n";

    }

    generateStatusLine();
    generateHeaders();
    _raw_response += _content;
}

void Response::generatePostResponse() {

    std::cout << "CGI SCRIPT: " << _request->getCgiScriptPathForRequest() << std::endl;
	if (!isMethodAllowed() && (_request->getCgiScriptPathForRequest()).empty()) {
		_allow = getAllowHeader();
		return _request->setStatusCode(405);
	}

//TODO: need to figure out what path to use instead of root
	std::string filename = _request->getAbsoluteRootPathForRequest();
	_appendRequestTarget(filename, _request);

	_file_ext = _getExt(filename);
	if (_isCgiExt()) {
		_runCgi(filename);
		_parseHeadersFromCgiResponse();
		if (!_request->isStatusCodeOk())
			return ;
		if (_cgi_headers.count("content-length")) {
			_cgi_response.resize(libft::strtoul_base(_cgi_headers["content-length"], 10));
		}
		_content.swap(_cgi_response);
	}
	if (!_request->isStatusCodeOk())
		return ;

	generateStatusLine();
	generateHeaders();

	_raw_response += _content;

}

void Response::generateResponse() {
	if (_request->isStatusCodeOk()) {
	    try
        {
            _request->checkToClientMaxBodySize(_request->_content.size()); // 413 set inside if needed
            checkForAcceptPrefixHeaders();

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
        }
	    catch (WebServ::NotOKStatusCodeException& e) {}
	}

	if (!_request->isStatusCodeOk()) {
		generateResponseByStatusCode();
	}
}

void Response::sendResponse() {
	static int i = 0;

	long ret = 0;
		ret = send(_socket, _raw_response.c_str() + sent_len, remains, 0);
		std::cout << _raw_response.substr(0,200) << std::endl;
		if (ret >= 0) {
			sent_len += ret;
			remains -= ret;
			if (remains == 0) {
				++i;
				std::cout << "response " << i << " is sent" << std::endl;
				in_progress = false;
			} else {
				in_progress =  true;
			}
		} else if (ret == -1) {
			_request->_close_connection = true;
			in_progress = false;
		}



		std::cout << _raw_response.substr(0, 200) << std::endl; // skarry

}


void Response::checkForAcceptPrefixHeaders(void) {
    if (_request->_headers.count("accept-charset")) {
        _request->handleAcceptCharsetHeader();
    }

    if (_request->_headers.count("accept-language")) {
        _request->handleAcceptLanguageHeader(true);
    } else {
        _request->handleAcceptLanguageHeader(false);
    }
}


void Response::setRemains() {
	remains = _raw_response.size();
}
/*
PUT /nginx_meme.jpg HTTP/1.1

Host: localhost:8080

User-Agent: curl/7.75.0

Accept: *//*

Content-Length: 172172

Expect: 100-continue

 */