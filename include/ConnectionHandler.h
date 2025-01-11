#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <iostream>
#include <thread>
#include <filesystem>
#include <fstream>

#include "NetworkingInterface.h"

class ConnectionHandler {
public:
    explicit ConnectionHandler(NetworkingInterface &networkingInterface);

    void checkForConnections();

    void connectTo(std::string &ip, int &port) const;

    void getAvailableFiles();

    void sendFile(const std::string &filename);

    void receiveFile(const std::string &filename, const int &fileSize);

private:
    void handleConnection(const unsigned long long &clientSock) const;


    NetworkingInterface &ni;
    const std::filesystem::path defaultPath{"C:/dev/cpp/w1nja2/files"};
};


#endif //CONNECTION_HANDLER_H
