#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler(NetworkingInterface &networkingInterface) : ni(networkingInterface) {
}

void ConnectionHandler::checkForConnections() {
    while (true) {
        std::thread t(&ConnectionHandler::handleConnection, this, ni.acceptSocketConnection());
        t.detach(); // maybe better way of handling
    }
    // check for new connections, if found handle them in thread, maybe while loop with a select call
}


void ConnectionHandler::handleConnection(const unsigned long long &clientSock) const {
    std::cout << "Client Socket: " << clientSock << std::endl;
    char recvbuf[1024];
    int bytes;
    do {
        memset(recvbuf, 0, sizeof(recvbuf));
        if ((bytes = ni.receiveDataTCP(clientSock, recvbuf, 1024)) > 0) {
            std::cout << "Bytes received: " << bytes << std::endl;
            std::cout << recvbuf << std::endl;
        } else if (bytes == 0) {
            std::cout << "Connection closed\n";
        } else {
            throw std::runtime_error("recv failed: " + std::string(strerror(errno)));
        }
    } while (bytes > 0);
    //wait for message from client
    while (true) {
    }
}

void ConnectionHandler::connectTo(std::string &ip, int &port) const {
    ni.connectToSocket(ip, port);
    //now that peer1 and peer2 are connected, client sends msg
    std::string sendbuf = "Hello, World!\n";
    int bytes;
    if ((bytes = ni.sendDataTCP(ni.getTCPClientSocket(), sendbuf.c_str(), sendbuf.size())) == -1) {
        throw std::runtime_error("send failed: " + std::string(strerror(errno)));
    }
    std::cout << "Bytes sent: " << bytes << std::endl;
}


