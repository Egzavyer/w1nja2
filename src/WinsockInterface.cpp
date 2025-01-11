#include "WinsockInterface.h"

WinsockInterface::WinsockInterface() {
    udpSocket = INVALID_SOCKET;
    tcpServerSocket = INVALID_SOCKET;
    tcpClientSocket = INVALID_SOCKET;
    udpPort = 8080;
    tcpServerPort = 8081;
    tcpClientPort = 8082;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(udpPort);
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
}

void WinsockInterface::startup() {
    initialise();
    createUDPSocket();
    createTCPServerSocket();
    createTCPClientSocket();
    bindUDPSocket();
    bindTCPServerSocket();
    listenOnSocket();
    setOwnIp();
}

void WinsockInterface::initialise() {
    WSADATA wsaData;

    if (const int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); iResult != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
    }
}

void WinsockInterface::createUDPSocket() {
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        throw std::runtime_error("udp socket failed: " + std::to_string(WSAGetLastError()));
    }
}

void WinsockInterface::createTCPServerSocket() {
    tcpServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpServerSocket == INVALID_SOCKET) {
        throw std::runtime_error("tcp server socket failed: " + std::string(strerror(errno)));
    }
}

void WinsockInterface::createTCPClientSocket() {
    if ((tcpClientSocket = socket(AF_INET,SOCK_STREAM, 0)) == INVALID_SOCKET) {
        throw std::runtime_error("tcp client socket failed: " + std::string(strerror(errno)));
    }
}

void WinsockInterface::bindUDPSocket() {
    struct sockaddr_in udpAddr{AF_INET, htons(udpPort)};
    udpAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(udpSocket, reinterpret_cast<sockaddr *>(&udpAddr), sizeof(udpAddr))) {
        throw std::runtime_error("udp bind failed: " + std::string(strerror(errno)));
    }

    int optval = 1;
    if ((setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&optval),
                    sizeof(optval))) == INVALID_SOCKET) {
        throw std::runtime_error("udp setsockopt failed: " + std::string(strerror(errno)));
    }
}

void WinsockInterface::bindTCPServerSocket() {
    struct sockaddr_in tcpAddr{AF_INET, htons(tcpServerPort)};

    int optval = 1;
    if ((setsockopt(tcpServerSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&optval), sizeof(optval))) == -
        1) {
        throw std::runtime_error("udp setsockopt failed: " + std::string(strerror(errno)));
    }

    tcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(tcpServerSocket, reinterpret_cast<sockaddr *>(&tcpAddr), sizeof(tcpAddr))) {
        throw std::runtime_error("tcp bind failed: " + std::string(strerror(errno)));
    }
}

void WinsockInterface::listenOnSocket() {
    if (listen(tcpServerSocket, 3) == INVALID_SOCKET) {
        throw std::runtime_error("listen failed: " + std::string(strerror(errno)));
    }
}

std::pair<std::string, int> WinsockInterface::receiveDataUDP(char *recvbuf, int recvbuflen) {
    //return address and port of sender
    int bytesReceived;
    char ipbuf[INET_ADDRSTRLEN];
    struct sockaddr_in peerAddr{};
    socklen_t peeraddrlen = sizeof(struct sockaddr_in);
    if ((bytesReceived = recvfrom(udpSocket, recvbuf, recvbuflen, 0, reinterpret_cast<sockaddr *>(&peerAddr),
                                  &peeraddrlen)) == INVALID_SOCKET) {
        throw std::runtime_error("recvfrom failed: " + std::string(strerror(errno)));
    }
    recvbuf[bytesReceived] = '\0';
    inet_ntop(AF_INET, &(peerAddr.sin_addr), ipbuf, INET_ADDRSTRLEN);

    std::pair<std::string, int> peerInfo{std::string(ipbuf), stoi(std::string(recvbuf).substr(1))};
    return peerInfo;
}

void WinsockInterface::broadcast(std::string sendbuf, int sendbuflen) {
    if ((sendto(udpSocket, sendbuf.c_str(), sendbuflen, 0, reinterpret_cast<sockaddr *>(&broadcastAddr),
                sizeof(broadcastAddr))) ==
        INVALID_SOCKET) {
        throw std::runtime_error("sendto failed: " + std::string(strerror(errno)));
    }
}

void WinsockInterface::connectToSocket(std::string &ip, int &port) {
    struct sockaddr_in addr{};
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

const unsigned long long WinsockInterface::acceptSocketConnection() {
    struct sockaddr_in address{};
    socklen_t addrlen = sizeof(address);
    SOCKET clientSock = INVALID_SOCKET;
    if ((clientSock = accept(tcpServerSocket, reinterpret_cast<struct sockaddr *>(&address), &addrlen)) ==
        INVALID_SOCKET) {
        throw std::runtime_error("accept failed: " + std::string(strerror(errno)));
    }
    char peerIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), peerIP, INET_ADDRSTRLEN);
    std::cout << "Accepted connection from " << peerIP << std::endl;
    return clientSock;
}

int WinsockInterface::sendDataTCP(const unsigned long long &socket, const char *sendbuf, int sendbuflen) {
    return send(static_cast<int>(socket), sendbuf, sendbuflen, 0);
}

int WinsockInterface::receiveDataTCP(const unsigned long long &socket, char *recvbuf, int recvbuflen) {
    return recv(socket, recvbuf, recvbuflen, 0);
}

unsigned long long WinsockInterface::getUDPSocket() {
    return udpSocket;
}

unsigned long long WinsockInterface::getTCPServerSocket() {
    return tcpServerSocket;
}

unsigned long long WinsockInterface::getTCPClientSocket() {
    return tcpClientSocket;
}

int WinsockInterface::getUDPPort() {
    return udpPort;
}

int WinsockInterface::getTCPServerPort() {
    return tcpServerPort;
}

int WinsockInterface::getTCPClientPort() {
    return tcpClientPort;
}

std::string &WinsockInterface::getOwnIp() {
    return ownIP;
}

void WinsockInterface::setOwnIp() {
    struct addrinfo *result = nullptr;
    struct addrinfo hints{};

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char hostname[256];
    gethostname(hostname, sizeof(hostname));

    if (getaddrinfo(hostname, "0", &hints, &result) != 0) {
        throw std::runtime_error("getaddrinfo failed: " + std::string(strerror(errno)));
    }
    const struct sockaddr_in *sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>(result->ai_addr);
    ownIP = inet_ntoa(sockaddr_ipv4->sin_addr);
    std::cout << "IP Address is: " << ownIP << std::endl;
}
