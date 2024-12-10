#include "LinuxInterface.h"

#include <cstring>

LinuxInterface::LinuxInterface() {
    udpSocket = -1;
    tcpServerSocket = -1;
    tcpClientSocket = -1;
    udpPort = 8080;
    tcpServerPort = 8081;
    tcpClientPort = 8082;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(udpPort);
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
}

LinuxInterface::~LinuxInterface() = default;

void LinuxInterface::startup() {
    initialise();
    createUDPSocket();
    createTCPServerSocket();
    createTCPClientSocket();
    bindUDPSocket();
    bindTCPServerSocket();
    listenOnSocket();
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

void LinuxInterface::createTCPServerSocket() {
    tcpServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpServerSocket == -1) {
        throw std::runtime_error("tcp server socket failed: " + std::string(strerror(errno)));
    }
}

void LinuxInterface::createTCPClientSocket() {
    if ((tcpClientSocket = socket(AF_INET,SOCK_STREAM, 0)) == -1) {
        throw std::runtime_error("tcp client socket failed: " + std::string(strerror(errno)));
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

void LinuxInterface::bindTCPServerSocket() {
    struct sockaddr_in tcpAddr{AF_INET, htons(tcpServerPort)};

    int optval = 1;
    if ((setsockopt(tcpServerSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *) &optval, sizeof(optval))) == -
        1) {
        throw std::runtime_error("udp setsockopt failed: " + std::string(strerror(errno)));
    }

    tcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(tcpServerSocket, reinterpret_cast<sockaddr *>(&tcpAddr), sizeof(tcpAddr))) {
        throw std::runtime_error("tcp bind failed: " + std::string(strerror(errno)));
    }
}

void LinuxInterface::listenOnSocket() {
    if (listen(tcpServerSocket, 3) == -1) {
        throw std::runtime_error("listen failed: " + std::string(strerror(errno)));
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

void LinuxInterface::connectToSocket(std::string &ip, int &port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        throw std::runtime_error("inet_pton failed: " + std::string(strerror(errno)));
    }

    if (connect(tcpClientSocket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        throw std::runtime_error("connect failed: " + std::string(strerror(errno)));
    }
    char peerIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), peerIP, INET_ADDRSTRLEN);
    std::cout << "Connected to: " << peerIP << std::endl;
}

const unsigned long long LinuxInterface::acceptSocketConnection() {
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int clientSock = -1;
    if ((clientSock = accept(tcpServerSocket, reinterpret_cast<struct sockaddr *>(&address), &addrlen)) == -1) {
        throw std::runtime_error("accept failed: " + std::string(strerror(errno)));
    }
    char peerIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), peerIP, INET_ADDRSTRLEN);
    std::cout << "Accepted connection from " << peerIP << std::endl;
    return clientSock;
}


unsigned long long LinuxInterface::getUDPSocket() {
    return udpSocket;
}

unsigned long long LinuxInterface::getTCPServerSocket() {
    return tcpServerSocket;
}

unsigned long long LinuxInterface::getTCPClientSocket() {
    return tcpClientSocket;
}

int LinuxInterface::getUDPPort() {
    return udpPort;
}

int LinuxInterface::getTCPServerPort() {
    return tcpServerPort;
}

int LinuxInterface::getTCPClientPort() {
    return tcpClientPort;
}
