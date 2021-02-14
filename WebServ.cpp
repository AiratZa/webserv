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

        /*!!!CAN TEST LOADING FROM CONFIG
         * ./webserv config_parsing/test_configs/2.conf
         * PLEASE DONT CHANGE CONFIG FILE STRUCTURE
        */
         _config = Config(config_file_path);

        std::cout << std::endl << "===================================================" << std::endl << std::endl;

        const std::list<ServerContext*>& servers = _config.getServersList();
        std::list<ServerContext*>::const_iterator it = servers.begin();
        std::list<ServerContext*>::const_iterator ite = servers.end();

        int i = 1;
        while (it != ite) {
            std::cout << "+++++ SERVER #" << i << " +++++" << std::endl;
            std::cout << "HOST AND PORTS: " << (*it)->getHostsAndPorts() << std::endl;
            std::cout << "____________________________________________" << std::endl;
            std::cout << "SERVER NAMES: "  << (*it)->getServerNames() << std::endl;
            std::cout << "____________________________________________" << std::endl;
            std::cout << "SERVER LEVEL ERROR_PAGES: "  << (*it)->getErrorPagesDirectiveInfo() << std::endl;
            std::cout << "____________________________________________" << std::endl;

            Server *temp = new Server(*(*it));
            delete *it;
            addServer(temp);
            ++it;
            i++;
        }

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
    _max_fd = 0;
    for(std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        int tmp = (*it)->getMaxFD();
        if (tmp > _max_fd)
            _max_fd = tmp;
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

void WebServ::serveConnections() {
	while(TRUE) {

		FD_ZERO(getReadSetPtr());
		FD_ZERO(getWriteSetPtr());

		for (int i = 0; i < getServersCount(); i++) {
			Server *server = getServerByPosition(i);
			FD_SET(server->getListener(), getReadSetPtr());
			setToReadFDSet(server->getReadClients());
			setToWriteFDSet(server->getReadClients());
			server->updateMaxFD();
		}

		updateMaxFD();

		// Ждём события в одном из сокетов
		if (select(getMaxFD() + 1,
				   getReadSetPtr(),
				   getWriteSetPtr(),
				   NULL,
				   NULL) < 0) {
			utils::exitWithLog();
		}

		// Определяем тип события и выполняем соответствующие действия
		for (int i = 0; i < getServersCount(); i++) {
			Server *server = getServerByPosition(i);
			if (FD_ISSET(server->getListener(), getReadSetPtr())) {
				// Поступил новый запрос на соединение, используем accept
				server->acceptConnection();
			}

			server->processConnections(getReadSetPtr(), getWriteSetPtr());
		}

	}
}

void WebServ::stop() {
	return ;
}



