#include "PeerDiscovery.h"

PeerDiscovery::PeerDiscovery(NetworkingInterface &ni) : n(ni) {

    broadcastRequest();
}

void PeerDiscovery::discoverPeers() {
    std::cout << "UDP Socket: " << n.getUDPSocket() << std::endl;
    std::cout << "TCP Socket: " << n.getTCPSocket() << std::endl;
    char recvbuf[1025];
    int recvbuflen = 1024;

    while (true) { //TODO: put in a while with a flag for interrupting or do polling or select
        //while (getPeerTable().size() < 2) {
        std::pair<std::string, int> peerInfo = n.receiveDataUDP(recvbuf, recvbuflen);
        if (peerTable.find(peerInfo.first) == peerTable.end()) { //TODO: find a way to remove or not add own ip
            addPeer(peerInfo.first, peerInfo.second);
            if (recvbuf[0] == '0') {
                broadcastResponse(); //TODO: maybe send info directly to requester's udp port
            }
        }
    }
}

void PeerDiscovery::broadcastRequest() {
    std::string sendbuf = "0" + std::to_string(n.getTCPPort());
    n.broadcast(sendbuf, (int) sendbuf.size());
}

void PeerDiscovery::broadcastResponse() {
    std::string sendbuf = "1" + std::to_string(n.getTCPPort());
    n.broadcast(sendbuf, (int) sendbuf.size());
}

void PeerDiscovery::addPeer(const std::string &ipAddress, const int &port) {
    std::lock_guard<std::mutex> lock(peerTableMutex);
    std::cout << "Peer at: " << ipAddress << "::" << port << std::endl;
    peerTable[ipAddress] = port;
}


std::unordered_map<std::string, int> PeerDiscovery::getPeerTable() {
    std::lock_guard<std::mutex> lock(peerTableMutex);
    return peerTable;
}
