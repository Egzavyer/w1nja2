#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <iostream>
#include <thread>

#include "NetworkingInterface.h"

class ConnectionHandler {
public:
    explicit ConnectionHandler(NetworkingInterface &networkingInterface);

    void checkForConnections();

    void connectTo(std::string &ip, int &port);

private:
    void handleConnection(const unsigned long long &clientSock);

    NetworkingInterface &ni;
};


#endif //FILE_HANDLER_H
