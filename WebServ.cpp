//
// Created by airat on 06.02.2021.
//

#include "WebServ.hpp"

WebServ::WebServ(const std::string& config_file_path) {
    if (config_file_path == CONFIG_FILE_DEFAULT_PATH) { //TODO: //TEMP RESOLUTION, NNED TO BE CORRECTED
        addServer(Server(8180));
        addServer(Server(8181));
//        addServer(Server(8082));
    } else {
        addServer(Server(8088));
    }
//    addServersListenerFD();
    std::cout << "Server(s) are started" << std::endl;
}


void WebServ::addServer(const Server& server) {
    _servers.push(server);
}

Server& WebServ::getFrontServer(void) {
    return _servers.front();
}

void WebServ::popFrontServer(void) {
    _servers.pop();
}


//void WebServ::addServersListenerFD(void) {
//    for (int i = 0; i < _servers.size(); i++) {
//        _serversListenerFD.push_back(_servers[i].getListener());
//    }
//}



//Server& WebServ::getServerByPosition(int i) {
//    return _servers[i];
//}





