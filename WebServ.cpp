//
// Created by airat on 06.02.2021.
//

#include "WebServ.hpp"

WebServ::WebServ(const std::string& config_file_path) {
    if (config_file_path == CONFIG_FILE_DEFAULT_PATH) { //TODO: //TEMP RESOLUTION, NNED TO BE CORRECTED
        Server* one = new Server(8080);
        Server* two = new Server(8081);
        addServer(one);
        addServer(two);
    } else {
        Server *three = new Server(8888);
        addServer(three);
    }
    std::cout << "Server(s) are started" << std::endl;
}


void WebServ::addServer(Server* server) {
    _servers.push_back(server);
}

void WebServ::setToReadFDSet(std::list<int>& clientsFD) {
    for(std::list<int>::iterator it = clientsFD.begin(); it != clientsFD.end(); it++)
        FD_SET(*it, &_readset);
}

void WebServ::setToWriteFDSet(std::list<int>& clientsFD) {
    for(std::list<int>::iterator it = clientsFD.begin(); it != clientsFD.end(); it++)
        FD_SET(*it, &_writeset);
}

void WebServ::updateMaxFD(void) {
    _maxFD = 0;
    for(std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        int tmp = (*it)->getMaxFD();
        if (tmp > _maxFD)
            _maxFD = tmp;
    }
}






//void WebServ::addServersListenerFD(void) {
//    for (int i = 0; i < _servers.size(); i++) {
//        _serversListenerFD.push_back(_servers[i].getListener());
//    }
//}



Server* WebServ::getServerByPosition(int i) {
    return _servers[i];
}





