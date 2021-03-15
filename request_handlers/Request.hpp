#ifndef REQUEST_HPP
# define REQUEST_HPP

class Request;
#include <map>
#include <set>
#include <string>
#include <climits>
#include "../WebServ.hpp"
#include "../response/Response.hpp"
#include "../config_parsing/Config.hpp"

#define MAX_HEADER_LINE_LENGTH 8192 //http://nginx.org/en/docs/http/ngx_http_core_module.html#large_client_header_buffers
#define DEFAULT_REQUEST_STATUS_CODE 200

#define BUFFER_LENGHT 1024
#define TIME_OUT 200000


class Request {

private:
    static std::set<std::string>    initRequestHeaders();
    static std::list<int>           initOkStatusCodes(void);
    int                             _status_code;

public:
    static const std::set<std::string>  implemented_headers;
    static const std::list<int>         OK_STATUS_CODES;
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
    int                                 _bytes_read;
    std::string                         _full_filename;

private:
    std::size_t         _header_end_pos;
    bool                _header_was_read;
    bool                _is_file_exists;
    long long           _only_content_length_read_body_size;
    bool                _is_need_writing_body_to_file;
    std::string         cgi_script_path;
    std::string         _response_content_lang;

public:
    bool            is_chunked;
    std::string     _host;
    int             _port;
    std::size_t     _is_lang_file_pos;

    Request();
    Request(struct sockaddr_in & remote_addr, int server_port);
    ~Request(void);

    void setStatusCode(int status_code);
    void setStatusCodeNoExept(int status_code) { _status_code = status_code;}
    void setHandlingServer(ServerContext* handling_server) { _handling_server = handling_server;}
    void setHandlingLocation(LocationContext* location_to_route) { _handling_location = location_to_route;}
    void setAbsoluteRootPathForRequest(void);
    void setHeaderWasRead(void) { _header_was_read = true; }
    void setHeaderEndPos(std::size_t val) { _header_end_pos = val;}
    void setFileExistenceStatus(bool value) { _is_file_exists = value;}
    void setNeedWritingBodyToFile(bool value) { _is_need_writing_body_to_file = value;}
    void setCgiScriptPathForRequest(const std::string& path) { cgi_script_path = path;}
    void setHostAndPort(const std::string& host, const int port) { _host = host;  _port = port;}
    void setReponseContentLang(const std::string& lang) { _response_content_lang = lang;}

    std::string &           getRawRequest(void) { return this->_raw_request;}
    std::string             getAbsolutePathForPUTRequests(void) const;
    const std::string&      getAbsoluteRootPathForRequest(void) const { return _absolute_root_path_for_request;}
    int                     getStatusCode() { return _status_code;}
    long long               getOnlyContentLengthReadBodySize(void) { return _only_content_length_read_body_size;}
    bool                    getFileExistenceStatus(void) const { return _is_file_exists;}
    bool                    getNeedWritingBodyToFile(void) const { return _is_need_writing_body_to_file;}
    const std::string&      getReponseContentLang(void) { return _response_content_lang; }
    const std::string&      getCgiScriptPathForRequest(void) const { return cgi_script_path;}

    void parseRequestLine(void);
	void parsURL();
    void parseHeaders(void);

    void handleExpectHeader(void);
    void handleAcceptCharsetHeader(void);
    void handleAcceptLanguageHeader(bool is_header_exists);
    void increaseOnlyContentLengthReadBodySize(int bytes_read) { _only_content_length_read_body_size += bytes_read;}


    bool isHeaderWasRead(void) const { return _header_was_read; }
    bool isStatusCodeOk() {
        std::list<int>::const_iterator found = std::find(OK_STATUS_CODES.begin(), OK_STATUS_CODES.end(), _status_code);

        if (found == OK_STATUS_CODES.end()) {
            return false;
        }
        return true;
    }

    bool checkToClientMaxBodySize(void) {
        long long client_max_body_size;
        if (_handling_location) {
            client_max_body_size = _handling_location->getClientMaxBodySizeInfo();
        } else {
            client_max_body_size = _handling_server->getClientMaxBodySizeInfo();
        }

        std::map<std::string, std::string>::const_iterator found = _headers.find("content-length");
        if (found != _headers.end()) {
            long long content_length = libft::stoll_base(_headers["content-length"], 10);
            if (client_max_body_size && content_length > client_max_body_size) {
                setStatusCode(413);
                return false;
            }
        }
        return true;
    }

    bool checkToClientMaxBodySize(long long int value_to_check) {
        long long client_max_body_size;
        if (_handling_location) {
            client_max_body_size = _handling_location->getClientMaxBodySizeInfo();
        } else {
            client_max_body_size = _handling_server->getClientMaxBodySizeInfo();
        }


        if (client_max_body_size && (value_to_check > client_max_body_size)) {
            setStatusCode(413);
            return false;
        }
        return true;
    }

    bool writeBodyReadBytesIntoFile() {
        int file = open(_full_filename.c_str(), O_RDWR | O_TRUNC, 0666);
        if (file <= 0) {
            _status_code = 500;
            return false;
        }

        write(file, _content.c_str(), _content.size());
        _content.clear();
        close(file);
        return true;
    }

    bool checkIsMayFileBeOpenedOrCreated(void) {
        int flags;
        if (_is_file_exists) {
            flags = O_RDWR | O_TRUNC;
        }
        else {
            flags = O_RDWR | O_CREAT;
        }

        int file = open(_full_filename.c_str(), flags, 0666);
        if (file <= 0) {
            _status_code = 500;
            return false;
        }
        close(file);
        return true;
    }

    bool isFileExists(void) {
        struct stat buffer;
        return (stat (_full_filename.c_str(), &buffer) == 0);
    }

    bool isFileExists(const std::string& full_filename) {
        struct stat buffer;
        return (stat (full_filename.c_str(), &buffer) == 0);
    }

    bool isRegFileExists(const std::string& full_filename) {
        struct stat buffer;
        return ((stat (full_filename.c_str(), &buffer) == 0) && S_ISREG(buffer.st_mode));
    }

    bool isConcreteHeaderExists(const std::string& header_name) {
        if (_headers.find(header_name) == _headers.end()) {
            return false;
        }
        return true;
    }

    bool targetIsFile(void) {
        struct stat info_buf;

        if (stat(_full_filename.c_str(), &info_buf) == -1) {
            std::cout << strerror(errno) << std::endl;
            _status_code = 500;
            return false;
        }

        int file_type = info_buf.st_mode & S_IFMT;

        if (file_type == S_IFREG) // return true if it's file
            return true;
        return false;
    }
};

bool isMethodLimited(const LocationContext& handling_location, const std::string& method);

#endif
