//
// Created by airat on 06.02.2021.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP

#include <vector>

#include "Server.hpp"
#include "config_parsing/Config.hpp"

#define CONFIG_FILE_DEFAULT_PATH "/webserv.conf"


class WebServ {
public:
    WebServ() { }
    WebServ(const std::string& config_file_path);
    void addServer(Server* server);
    int getServersCount(void) const { return _servers.size(); }
    Server* getServerByPosition(int i);

    fd_set* getReadSetPtr(void) { return &_readset; };
    fd_set* getWriteSetPtr(void) { return &_writeset; };

    void setToReadFDSet(std::list<int>& clientsFD);
    void setToWriteFDSet(std::list<int>& clientsFD);

    void updateMaxFD(void);
    const int & getMaxFD() const { return _max_fd; }

	void serveConnections();

	void stop();



private:
    std::vector<Server*> _servers;

    // Для заполнения множества сокетов
    fd_set _readset;
    fd_set _writeset;
    int _max_fd;

    Config _config;

};


#endif //WEBSERV_WEBSERV_HPP
