#include "FileHandler.h"

//SENDER
// - take filepath
// - read 8192 bytes from the file into memory
// - once limit reached, send buffer to peer
// - resume reading until entire file is sent

//RECEIVER
// - take file size
// - writes incoming bytes to memory until target amount is received

FileHandler::FileHandler(NetworkingInterface &networkingInterface) : ni(networkingInterface) {
}

void FileHandler::getAvailableFiles() {
    for (auto const &dirEntry: std::filesystem::directory_iterator{defaultPath}) {
        std::cout << dirEntry.path().filename() << '\n';
    }
}

void FileHandler::sendFile(const std::string &filename) {
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

void FileHandler::receiveFile(const std::string &filename, const int &fileSize) {
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

