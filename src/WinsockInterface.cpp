#include "WinsockInterface.h"

WinsockInterface::WinsockInterface() {
    udpSocket = INVALID_SOCKET;
    tcpSocket = INVALID_SOCKET;
    udpPort = 8080;
    tcpPort = 8081;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(udpPort);
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
}

void WinsockInterface::startup() {
    initialise();
    createUDPSocket();
    createTCPSocket();
    bindUDPSocket();
    bindTCPSocket();
}

void WinsockInterface::initialise() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
    }
}

void WinsockInterface::createUDPSocket() {
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        throw std::runtime_error("udp socket failed: " + std::to_string(WSAGetLastError()));
    }
}

void WinsockInterface::createTCPSocket() {
    tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSocket == INVALID_SOCKET) {
        throw std::runtime_error("tcp socket failed: " + std::to_string(WSAGetLastError()));
    }
}

void WinsockInterface::bindUDPSocket() {
    struct sockaddr_in udpAddr{AF_INET, htons(udpPort)};
    udpAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(udpSocket, (SOCKADDR *) &udpAddr, sizeof(udpAddr))) {
        throw std::runtime_error("udp bind failed: " + std::to_string(WSAGetLastError()));
    }

    int optval = 1;
    if ((setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(optval))) == SOCKET_ERROR) {
        throw std::runtime_error("udp setsockopt failed: " + std::to_string(WSAGetLastError()));
    }
}

void WinsockInterface::bindTCPSocket() {
    struct sockaddr_in tcpAddr{AF_INET, htons(tcpPort)};

    tcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(tcpSocket, (SOCKADDR *) &tcpAddr, sizeof(tcpAddr))) {
        throw std::runtime_error("tcp bind failed: " + std::to_string(WSAGetLastError()));
    }
}

std::pair<std::string, int> WinsockInterface::receiveDataUDP(char *recvbuf, int recvbuflen) {
    //return address and port of sender
    int bytesReceived;
    char ipbuf[INET_ADDRSTRLEN];
    struct sockaddr_in peerAddr{};
    int peeraddrlen = sizeof(peerAddr);
    if ((bytesReceived = recvfrom(udpSocket, recvbuf, recvbuflen, 0, (SOCKADDR *) &peerAddr, &peeraddrlen)) ==
        SOCKET_ERROR) {
        throw std::runtime_error("recvfrom failed: " + std::to_string(WSAGetLastError()));
    }
    recvbuf[bytesReceived] = '\0';
    inet_ntop(AF_INET, &(peerAddr.sin_addr), ipbuf, INET_ADDRSTRLEN);

    std::pair<std::string, int> peerInfo{std::string(ipbuf), stoi(std::string(recvbuf).substr(1))};
    return peerInfo;
}

void WinsockInterface::broadcast(std::string sendbuf, int sendbuflen) {
    if ((sendto(udpSocket, sendbuf.c_str(), sendbuflen, 0, (SOCKADDR *) &broadcastAddr, sizeof(broadcastAddr))) ==
        SOCKET_ERROR) {
        throw std::runtime_error("sendto failed: " + std::to_string(WSAGetLastError()));
    }
}

unsigned long long WinsockInterface::getUDPSocket() {
    return udpSocket;
}

unsigned long long WinsockInterface::getTCPSocket() {
    return tcpSocket;
}

int WinsockInterface::getUDPPort() {
    return udpPort;
}

int WinsockInterface::getTCPPort() {
    return tcpPort;
}
