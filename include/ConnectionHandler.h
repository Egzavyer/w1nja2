#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <iostream>
#include <thread>

#include "NetworkingInterface.h"

class ConnectionHandler {
public:
    explicit ConnectionHandler(NetworkingInterface &networkingInterface);

    void checkForConnections();

    void connectTo(std::string &ip, int &port) const;

private:
    void handleConnection(const unsigned long long &clientSock) const;

    NetworkingInterface &ni;
};


#endif //CONNECTION_HANDLER_H
