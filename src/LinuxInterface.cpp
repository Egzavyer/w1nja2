#include "LinuxInterface.h"

#include <cstring>

LinuxInterface::LinuxInterface() {
    udpSocket = -1;
    tcpSocket = -1;
    udpPort = 8080;
    tcpPort = 8081;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(udpPort);
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
}

LinuxInterface::~LinuxInterface() = default;

void LinuxInterface::startup() {
    initialise();
    createUDPSocket();
    createTCPSocket();
    bindUDPSocket();
    bindTCPSocket();
}

void LinuxInterface::initialise() {
    /*WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::string(std::to_string(iResult)));
    }*/
}

void LinuxInterface::createUDPSocket() {
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        throw std::runtime_error("udp socket failed: " + std::string(strerror(errno)));
    }
}

void LinuxInterface::createTCPSocket() {
    tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket == -1) {
        throw std::runtime_error("tcp socket failed: " + std::string(strerror(errno)));
    }
}

void LinuxInterface::bindUDPSocket() {
    struct sockaddr_in udpAddr{AF_INET, htons(udpPort)};
    udpAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(udpSocket, reinterpret_cast<sockaddr *>(&udpAddr), sizeof(udpAddr))) {
        throw std::runtime_error("udp bind failed: " + std::string(strerror(errno)));
    }

    int optval = 1;
    if ((setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(optval))) == -1) {
        throw std::runtime_error("udp setsockopt failed: " + std::string(strerror(errno)));
    }
}

void LinuxInterface::bindTCPSocket() {
    struct sockaddr_in tcpAddr{AF_INET, htons(tcpPort)};

    tcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(tcpSocket, reinterpret_cast<sockaddr *>(&tcpAddr), sizeof(tcpAddr))) {
        throw std::runtime_error("tcp bind failed: " + std::string(strerror(errno)));
    }
}

std::pair<std::string, int> LinuxInterface::receiveDataUDP(char *recvbuf, int recvbuflen) {
    //return address and port of sender
    int bytesReceived;
    char ipbuf[INET_ADDRSTRLEN];
    struct sockaddr_in peerAddr{};
    socklen_t peeraddrlen = sizeof(struct sockaddr_in);;
    if ((bytesReceived = recvfrom(udpSocket, recvbuf, recvbuflen, 0, reinterpret_cast<sockaddr *>(&peerAddr),
                                  &peeraddrlen)) == -1) {
        throw std::runtime_error("recvfrom failed: " + std::string(strerror(errno)));
    }
    recvbuf[bytesReceived] = '\0';
    inet_ntop(AF_INET, &(peerAddr.sin_addr), ipbuf, INET_ADDRSTRLEN);

    std::pair<std::string, int> peerInfo{std::string(ipbuf), stoi(std::string(recvbuf).substr(1))};
    return peerInfo;
}

void LinuxInterface::broadcast(std::string sendbuf, int sendbuflen) {
    if ((sendto(udpSocket, sendbuf.c_str(), sendbuflen, 0, reinterpret_cast<sockaddr *>(&broadcastAddr),
                sizeof(broadcastAddr))) ==
        -1) {
        throw std::runtime_error("sendto failed: " + std::string(strerror(errno)));
    }
}

unsigned long long LinuxInterface::getUDPSocket() {
    return udpSocket;
}

unsigned long long LinuxInterface::getTCPSocket() {
    return tcpSocket;
}

int LinuxInterface::getUDPPort() {
    return udpPort;
}

int LinuxInterface::getTCPPort() {
    return tcpPort;
}
