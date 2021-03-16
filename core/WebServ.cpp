//
// Created by airat on 06.02.2021.
//

#include "WebServ.hpp"

WebServ::~WebServ(void) {
    stop();
}

void WebServ::stop() {
    std::vector<Server*>::iterator _servers_it = _servers.begin();

    while (_servers_it != _servers.end()) {
        delete *_servers_it;
        ++_servers_it;
    }
}

bool WebServ::isAlreadyListeningHostPlusPort(const std::string& host_str, int port) {
    std::string all_hosts = "*";
    if (already_listening_host_plus_port.count(all_hosts))
    {
        if ((std::find(already_listening_host_plus_port[all_hosts].begin(),
                       already_listening_host_plus_port[all_hosts].end(),
                       port) != already_listening_host_plus_port[all_hosts].end()))
        {
            return true;
        }
    }

    if (!already_listening_host_plus_port.count(host_str))
    {
        return false;
    }
    else
    {
        if ((std::find(already_listening_host_plus_port[host_str].begin(),
                       already_listening_host_plus_port[host_str].end(),
                       port) == already_listening_host_plus_port[host_str].end()))
        {
            return false;
        }
        return true;
    }
}

void WebServ::addServer(Server* server) {
    _servers.push_back(server);
}

WebServ::WebServ() {
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
            std::cout << "      PARAM auth_enable: " << std::boolalpha << (*loc_it)->getAuthEnable() << std::endl;
            std::cout << "      ____________________________________________" << std::endl;
            std::cout << std::boolalpha << "IS CGI LOCATION: " << (*loc_it)->isExtLocation() << std::endl;
            if ((*loc_it)->isExtLocation()) {
                std::cout << "      CGI_PARAM cgi_script: "  << (*loc_it)->getCgiScript() << std::endl;
                std::cout << "      ____________________________________________" << std::endl;
                const std::string& ext = (*loc_it)->getLocationExtension();
                if (!ext.empty()) {
                    std::cout << "      CGI_PARAM cgi_exts: " << ext << std::endl;
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

void WebServ::serveConnections() {
	fd_set temp_read_set;
	fd_set temp_write_set;
	struct   timeval tv;
	tv.tv_sec = 30;
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

//                std::cout << "_clients_read ";
//                for (std::list<int>::const_iterator it = (*it_l)->getReadClients().begin(); it != (*it_l)->getReadClients().end(); ++it) {
//					std::cout << *it << " ";
//                }
//				std::cout << std::endl;
//				std::cout << "_clients_write ";
//				for (std::list<int>::const_iterator it = (*it_l)->getWriteClients().begin(); it != (*it_l)->getWriteClients().end(); ++it) {
//					std::cout << *it << " ";
//				}
//				std::cout << std::endl;

                (*it_l)->processConnections(&temp_read_set, &temp_write_set);
                ++it_l;
            }
		}
	}
}




