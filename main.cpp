#include "Server.hpp"
#include "WebServ.hpp"

WebServ webServ;

void serveConnections(WebServ& webServ) {
    while(TRUE) {

        FD_ZERO(webServ.getReadSetPtr());
        FD_ZERO(webServ.getWriteSetPtr());

        for (int i = 0; i < webServ.getServersCount(); i++) {
            Server *server = webServ.getServerByPosition(i);
            FD_SET(server->getListener(), webServ.getReadSetPtr());
            webServ.setToReadFDSet(server->getReadClients());
            webServ.setToWriteFDSet(server->getReadClients());
            server->updateMaxFD();
        }

        webServ.updateMaxFD();

        // Ждём события в одном из сокетов
        if (select(webServ.getMaxFD() + 1,
                   webServ.getReadSetPtr(),
                   webServ.getWriteSetPtr(),
                   NULL,
                   NULL) < 0) {
            utils::exitWithLog();
            return ;
        }

        // Определяем тип события и выполняем соответствующие действия
        for (int i = 0; i < webServ.getServersCount(); i++) {
            Server *server = webServ.getServerByPosition(i);
            if (FD_ISSET(server->getListener(), webServ.getReadSetPtr())) {
                // Поступил новый запрос на соединение, используем accept
                server->acceptConnection();
            }

            server->processConnections(webServ.getReadSetPtr(), webServ.getWriteSetPtr());
        }

    }
}

void intHandler(int signal) {
	(void)signal;
	webServ.stop();
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    std::string path_to_config;
    if  (argc == 2) {
        path_to_config = argv[1];
    } else if (argc == 1) {
        path_to_config = CONFIG_FILE_DEFAULT_PATH;
    } else {
        std::cout << "Please provide only 1 arg -> path to webserv config" << std::endl\
            << "or dont provide nothing and will be used CONFIG_FILE_DEFAULT_PATH" << std::endl;
        exit(EXIT_FAILURE);
    }
	signal(SIGINT, intHandler);
    webServ = WebServ(path_to_config);
    serveConnections(webServ);
    return 0;
}
