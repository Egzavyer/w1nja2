#include "PeerDiscovery.h"

PeerDiscovery::PeerDiscovery(NetworkingInterface &ni) : n(ni) {
    n.broadcastRequest();
}

void PeerDiscovery::discoverPeers() {
    std::cout << "UDP Socket: " << n.getUDPSocket() << std::endl;
    std::cout << "TCP Socket: " << n.getTCPSocket() << std::endl;
    char recvbuf[1025];
    int recvbuflen = 1024;

    while (true) {
        //while (getPeerTable().size() < 2) {
        std::string peerInfo;
        peerInfo = n.receiveDataUDP(recvbuf, recvbuflen);
        std::cout << "Peer at: " << peerInfo << std::endl;
        if (recvbuf[0] == '0') {
            std::cout << "Request received 0\n";
        }
    }
    /*
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
    }*/
}

/*void PeerDiscovery::addPeer(const std::string &ipAddress, const int &port) {
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
}*/
