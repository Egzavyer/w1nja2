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
    char recvbuf[8192];
    int bytes;
    //TODO: fit ConnectionHandler::receiveFile logic here somehow
    do {
        memset(recvbuf, 0, sizeof(recvbuf));
        if ((bytes = ni.receiveDataTCP(clientSock, recvbuf, sizeof(recvbuf))) > 0) {
            std::cout << "Bytes received: " << bytes << std::endl;
            std::cout << recvbuf << std::endl;
        } else if (bytes == 0) {
            std::cout << "Connection closed\n";
        } else {
            throw std::runtime_error("recv failed: " + std::string(strerror(errno)));
        }
    } while (bytes > 0);
    //wait for message from client
    //while (true) {}
}

void ConnectionHandler::connectTo(std::string &ip, int &port) const {
    ni.connectToSocket(ip, port);
    //now that peer1 and peer2 are connected, client sends msg
    //TODO: fit ConnectionHandler::sendFile logic here somehow
    std::string sendbuf = "Hello, World!\n";
    int bytes;
    if ((bytes = ni.sendDataTCP(ni.getTCPClientSocket(), sendbuf.c_str(), sendbuf.size())) == -1) {
        throw std::runtime_error("send failed: " + std::string(strerror(errno)));
    }
    std::cout << "Bytes sent: " << bytes << std::endl;
}

//SENDER
// - take filepath
// - read 8192 bytes from the file into memory
// - once limit reached, send buffer to peer
// - resume reading until entire file is sent

//RECEIVER
// - take file size
// - writes incoming bytes to memory until target amount is received

void ConnectionHandler::getAvailableFiles() {
    for (auto const &dirEntry: std::filesystem::directory_iterator{defaultPath}) {
        std::cout << dirEntry.path().filename() << '\n';
    }
}

void ConnectionHandler::sendFile(const std::string &filename) {
    char sendbuf[8192];
    size_t bytesRemaining;
    size_t sendbuflen = 0;
    std::ifstream file(defaultPath / filename, std::ios::binary);

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    bytesRemaining = fileSize;
    while (bytesRemaining > 0) {
        if (bytesRemaining > 8191) {
            sendbuflen = 8191;
        } else {
            sendbuflen = bytesRemaining;
        }
        file.read(sendbuf, sendbuflen);
        sendbuf[sendbuflen] = '\0';
        ni.sendDataTCP(ni.getTCPClientSocket(), sendbuf, sendbuflen);
        //std::cout << sendbuf << '\n';
        bytesRemaining -= sendbuflen;
    }
    file.close();
}

void ConnectionHandler::receiveFile(const std::string &filename, const int &fileSize) {
    char recvbuf[8192];
    size_t bytesRemaining = fileSize;
    size_t totalBytesReceived = 0;

    std::ofstream file(defaultPath / filename, std::ios::binary);
    while (totalBytesReceived < fileSize) {
        const int bytesReceived = ni.receiveDataTCP(ni.getTCPClientSocket(), recvbuf, sizeof(recvbuf));
        file.write(recvbuf, bytesReceived);
        bytesRemaining -= bytesReceived;
        totalBytesReceived += bytesReceived;
    }
    file.close();
}



