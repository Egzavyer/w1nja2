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
    for (int i = 0; i < 2; i++) { //TODO: put in a while with a flag for interrupting or do polling
        bytesReceived = recvfrom(sock, recvbuf, recvbuflen, 0, (SOCKADDR *) &peerAddr, &peeraddrlen);
        if (bytesReceived == SOCKET_ERROR) {
            throw std::runtime_error("recvfrom failed: " + std::to_string(WSAGetLastError()));
        }
        recvbuf[bytesReceived] = '\0';
        if (recvbuf[0] == '0') {
            std::cout << "Request received\n";
            broadcastResponse();
        } else if (recvbuf[0] == '1') {
            char ipbuf[INET_ADDRSTRLEN];
            std::string port;

            for (int i = 1; i < bytesReceived; i++) {
                port += recvbuf[i];
            }
            inet_ntop(AF_INET, &(peerAddr.sin_addr), ipbuf, INET_ADDRSTRLEN);
            addPeer(ipbuf, stoi(port)); // TODO: send TCP port number in msg to use for file transfer
        }
    }
}

void PeerDiscovery::addPeer(const std::string &ipAddress, const int &port) {
    peerTable.push_back({ipAddress, port});
}

SOCKET PeerDiscovery::getSock() const {
    return sock;
}

std::vector<NearbyPeer> PeerDiscovery::getPeerTable() {
    return peerTable;
}

void PeerDiscovery::broadcastRequest() {
    char sendbuf[] = {'0', '\0'};
    int sendbuflen = (int) (sizeof(sendbuf) - 1);

    if ((sendto(sock, sendbuf, sendbuflen, 0, (SOCKADDR *) &broadcastAddr, sizeof(broadcastAddr))) == SOCKET_ERROR) {
        throw std::runtime_error("sendto failed: " + std::to_string(WSAGetLastError()));
    }
}

void PeerDiscovery::broadcastResponse() {
    char sendbuf[] = {'1', '8', '0', '8', '0', '\0'};
    int sendbuflen = (int) (sizeof(sendbuf) - 1);

    if ((sendto(sock, sendbuf, sendbuflen, 0, (SOCKADDR *) &broadcastAddr, sizeof(broadcastAddr))) == SOCKET_ERROR) {
        throw std::runtime_error("sendto failed: " + std::to_string(WSAGetLastError()));
    }
}

