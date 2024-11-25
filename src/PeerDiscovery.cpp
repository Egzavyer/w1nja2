#include "PeerDiscovery.h"

PeerDiscovery::PeerDiscovery() {
    initWinsock();
    sock = createSocket();
    bindSocket();
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    broadcastRequest();
}

void PeerDiscovery::initWinsock() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
    }
}

SOCKET PeerDiscovery::createSocket() {
    SOCKET udpSocket = INVALID_SOCKET;
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        throw std::runtime_error("socket failed: " + std::to_string(WSAGetLastError()));
    }
    return udpSocket;
}

void PeerDiscovery::bindSocket() {
    udpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (SOCKADDR *) &udpAddr, sizeof(udpAddr))) {
        throw std::runtime_error("bind failed: " + std::to_string(WSAGetLastError()));
    }
    int optval = 1;
    if ((setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(optval))) == SOCKET_ERROR) {
        throw std::runtime_error("setsockopt failed: " + std::to_string(WSAGetLastError()));
    }
}

void PeerDiscovery::discoverPeers() {
    int bytesReceived;
    char recvbuf[1025];
    int recvbuflen = 1024;
    int peeraddrlen = sizeof(peerAddr);
    // for (int i = 0; i < 2; i++) { //TODO: put in a while with a flag for interrupting or do polling
    while (getPeerTable().size() < 2) {
        bytesReceived = recvfrom(sock, recvbuf, recvbuflen, 0, (SOCKADDR *) &peerAddr, &peeraddrlen);
        if (bytesReceived == SOCKET_ERROR) {
            throw std::runtime_error("recvfrom failed: " + std::to_string(WSAGetLastError()));
        }
        recvbuf[bytesReceived] = '\0';
        if (recvbuf[0] == '0') {
            std::cout << "Request received 0\n";
            char ipbuf[INET_ADDRSTRLEN];
            std::string port;

            for (int i = 1; i < bytesReceived; i++) // TODO: maybe memcopy or something more efficient
            {
                port += recvbuf[i];
            }
            inet_ntop(AF_INET, &(peerAddr.sin_addr), ipbuf, INET_ADDRSTRLEN);
            if (peerTable.find(ipbuf) == peerTable.end()) {
                addPeer(ipbuf, stoi(port));
            }
            broadcastResponse();
        } else if (recvbuf[0] == '1') {
            std::cout << "Request received 1\n";
            char ipbuf[INET_ADDRSTRLEN];
            std::string port;

            for (int i = 1; i < bytesReceived; i++) // TODO: maybe memcopy or something more efficient
            {
                port += recvbuf[i];
            }
            inet_ntop(AF_INET, &(peerAddr.sin_addr), ipbuf, INET_ADDRSTRLEN);
            if (peerTable.find(ipbuf) == peerTable.end()) {
                addPeer(ipbuf, stoi(port));
            }
        }
    }
}

void PeerDiscovery::addPeer(const std::string &ipAddress, const int &port) {
    std::lock_guard<std::mutex> lock(peerTableMutex);
    std::cout << "Peer at: " << ipAddress << "::" << port << std::endl;
    peerTable[ipAddress] = port;
}

SOCKET PeerDiscovery::getSock() const {
    return sock;
}

std::unordered_map<std::string, int> PeerDiscovery::getPeerTable() {
    std::lock_guard<std::mutex> lock(peerTableMutex);
    return peerTable;
}

void PeerDiscovery::broadcastRequest() {
    std::string sendbuf = "0" + std::to_string(udpPort);
    int sendbuflen = (int) (sizeof(sendbuf) - 1);

    if ((sendto(sock, sendbuf.c_str(), sendbuflen, 0, (SOCKADDR *) &broadcastAddr, sizeof(broadcastAddr))) ==
        SOCKET_ERROR) {
        throw std::runtime_error("sendto failed: " + std::to_string(WSAGetLastError()));
    }
}

void PeerDiscovery::broadcastResponse() {
    // TODO: change this to tcp port
    std::string sendbuf = '1' + std::to_string(udpPort) + '\0';
    int sendbuflen = (int) (sizeof(sendbuf) - 1);

    if ((sendto(sock, sendbuf.c_str(), sendbuflen, 0, (SOCKADDR *) &peerAddr, sizeof(peerAddr))) ==
        SOCKET_ERROR) {
        throw std::runtime_error("sendto failed: " + std::to_string(WSAGetLastError()));
    }
}
