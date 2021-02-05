#include "Server.hpp"

int main()
{

    Server server(8880);

    while(TRUE)
    {

        FD_ZERO(server.getReadSetPtr());
        FD_ZERO(server.getWriteSetPtr());
        FD_SET(server.getListener(), server.getReadSetPtr());

        server.setToReadFDSet();
        server.setToWriteFDSet();

        // Ждём события в одном из сокетов
        server.updateMaxFD();
        if(select(server.getMaxFD() + 1,
                  server.getReadSetPtr(),
                  server.getWriteSetPtr(),
                  NULL,
                  NULL) < 0) {
            utils::exitWithLog();
        }

        // Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(server.getListener(), server.getReadSetPtr())) {
            // Поступил новый запрос на соединение, используем accept
            server.acceptConnection();
        }

        server.processConnections();

    }

    return 0;
}