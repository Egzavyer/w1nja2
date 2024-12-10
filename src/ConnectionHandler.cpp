#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler(NetworkingInterface &networkingInterface) : ni(networkingInterface) {
}

void ConnectionHandler::checkForConnections() {
    std::thread t(&ConnectionHandler::handleConnection, this, ni.acceptSocketConnection());
    // check for new connections, if found handle them in thread, maybe while loop with a select call
    while (true) {
    }
}


void ConnectionHandler::handleConnection(const unsigned long long &clientSock) {
    std::cout << "Client Socket: " << clientSock << std::endl;
    //wait for message from client
    while (true) {
    }
}

void ConnectionHandler::connectTo(std::string &ip, int &port) {
    ni.connectToSocket(ip, port);
    //now that peer1 and peer2 are connected, client sends msg
}


