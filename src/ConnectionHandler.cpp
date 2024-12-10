#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler(NetworkingInterface &networkingInterface) : ni(networkingInterface) {
}

void ConnectionHandler::checkForConnections() {
    std::cout << "Here\n";
    // check for new connections, if found handle them in thread
}


void ConnectionHandler::handleConnection() {
}

void ConnectionHandler::connectTo(std::string &ip, int &port) {
    ni.connectToSocket(ip, port);
}


