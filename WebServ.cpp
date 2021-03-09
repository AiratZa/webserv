//
// Created by airat on 06.02.2021.
//

#include "WebServ.hpp"

WebServ::WebServ(const Config& config)
            : _config(config) {
    std::list<ServerContext*>::const_iterator it = servers_list.begin();
    std::list<ServerContext*>::const_iterator ite = servers_list.end();

    int i = 1;
    while (it != ite) {
        std::cout << "+++++ SERVER #" << i << " +++++" << std::endl;
        std::cout << "HOST AND PORTS: " << (*it)->getHostsAndPorts() << std::endl;
        std::cout << "____________________________________________" << std::endl;
        std::cout << "SERVER NAMES: "  << (*it)->getServerNames() << std::endl;
        std::cout << "____________________________________________" << std::endl;
        std::cout << "SERVER LEVEL ERROR_PAGES: "  << (*it)->getErrorPagesDirectiveInfo() << std::endl;
        std::cout << "____________________________________________" << std::endl;
        std::cout << "SERVER LEVEL INDEX DIRECTIVE PARAMS: "  << (*it)->getIndexPagesDirectiveInfo() << std::endl;
        std::cout << "____________________________________________" << std::endl;

        const std::list<LocationContext*>& locations =  (*it)->getLocationsList();
        std::list<LocationContext*>::const_iterator loc_it = locations.begin();

        std::cout << "LOCATIONS URI IN SERVER: " << std::endl;
        while (loc_it != locations.end()) {
            std::cout << "      LOCATION [" << (*loc_it)->getLocationPath() << "]" << std::endl;
            std::cout << "      LOCATION LEVEL ERROR_PAGES: "  << (*loc_it)->getErrorPagesDirectiveInfo() << std::endl;
            std::cout << "      ____________________________________________" << std::endl;
            std::cout << "      LOCATION LEVEL INDEX DIRECTIVE PARAMS: "  << (*loc_it)->getIndexPagesDirectiveInfo() << std::endl;
            std::cout << "      ____________________________________________" << std::endl;
            std::cout << std::boolalpha << "IS CGI LOCATION: " << (*loc_it)->IsCgiLocation() << std::endl;
            if ((*loc_it)->IsCgiLocation()) {
                std::cout << "      CGI_PARAM cgi_script: "  << (*loc_it)->getCgiScript() << std::endl;
                std::cout << "      ____________________________________________" << std::endl;
                const std::list<std::string>& exts = (*loc_it)->getCgiExtensions();
                if (exts.size()) {
                    std::list<std::string>::const_iterator ext_it = exts.begin();
                    std::cout << "      CGI_PARAM cgi_exts: " ;

                    while (ext_it != exts.end()) {
                        std::cout << *ext_it << "  ";
                        ++ext_it;
                    }
                    std::cout << std::endl;
                }
                std::cout << "      ____________________________________________" << std::endl;
            }
            ++loc_it;
            std::cout << std::endl << "============================================" << std::endl;

        }
        std::cout << std::endl << "____________________________________________" << std::endl;

        Server *temp = new Server((*it));
        addServer(temp);
        ++it;
        i++;
    }

    std::cout << "Server(s) are started" << std::endl;
}


void WebServ::addServer(Server* server) {
    _servers.push_back(server);
}

//void WebServ::setToReadFDSet(const std::list<int>& clientsFD) {
//    for(std::list<int>::const_iterator it = clientsFD.begin(); it != clientsFD.end(); it++)
//        FD_SET(*it, &_readset);
//}
//
//void WebServ::setToWriteFDSet(const std::list<int>& clientsFD) {
//    for(std::list<int>::const_iterator it = clientsFD.begin(); it != clientsFD.end(); it++)
//        FD_SET(*it, &_writeset);
//}

void WebServ::updateMaxFD(void) {
    _max_fd = 0;
    for(std::vector<Server*>::iterator it = _servers.begin(); it != _servers.end(); it++) {
        std::list<Listener*> listeners = (*it)->getListeners();
        std::list<Listener*>::iterator it_l = listeners.begin();
        while (it_l != listeners.end()) {
            int tmp = (*it_l)->getMaxFD();
            if (tmp > _max_fd)
                _max_fd = tmp;
            ++it_l;
        }
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
	fd_set temp_read_set;
	fd_set temp_write_set;
	struct   timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	while(true) {

		FD_ZERO(&temp_read_set);
		FD_ZERO(&temp_write_set);

		for (int i = 0; i < getServersCount(); i++) {
			Server *server = getServerByPosition(i);

			std::list<Listener*> listeners = server->getListeners();
            std::list<Listener*>::iterator it_l = listeners.begin();
            while (it_l != listeners.end()) {
                FD_SET((*it_l)->getListener(), &temp_read_set);
				for(std::list<int>::const_iterator it = (*it_l)->getReadClients().begin(); it != (*it_l)->getReadClients().end(); it++)
					FD_SET(*it, &temp_read_set);
				for(std::list<int>::const_iterator it = (*it_l)->getWriteClients().begin(); it != (*it_l)->getWriteClients().end(); it++)
					FD_SET(*it, &temp_write_set);
                (*it_l)->updateMaxFD();
                ++it_l;
            }
		}

		updateMaxFD();
		// Ждём события в одном из сокетов
		if (select(getMaxFD() + 1,
				   &temp_read_set,
				   &temp_write_set,
				   NULL,
				   &tv) < 0) {
			utils::exitWithLog();
		}

		// Определяем тип события и выполняем соответствующие действия
		for (int i = 0; i < getServersCount(); i++) {
			Server *server = getServerByPosition(i);

            std::list<Listener*> listeners = server->getListeners();
            std::list<Listener*>::iterator it_l = listeners.begin();
            while (it_l != listeners.end()) {
                if (FD_ISSET((*it_l)->getListener(), &temp_read_set)) {
                    // Поступил новый запрос на соединение, используем accept
                    (*it_l)->acceptConnection();
                }
                (*it_l)->processConnections(&temp_read_set, &temp_write_set);
                ++it_l;
            }
		}
	}
}

void WebServ::stop() {
	return ;
}



