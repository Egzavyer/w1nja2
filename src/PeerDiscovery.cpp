#include "PeerDiscovery.h"

PeerDiscovery::PeerDiscovery(NetworkingInterface &networkingInterface) : ni(networkingInterface) {
    broadcastRequest();
}

void PeerDiscovery::discoverPeers() {
    std::cout << "UDP Socket: " << ni.getUDPSocket() << std::endl;
    std::cout << "TCP Socket: " << ni.getTCPServerSocket() << std::endl;

    while (true) {
        constexpr int recvbuflen = 1024;
        char recvbuf[1025];
        //TODO: put in a while with a flag for interrupting or do polling or select
        //while (getPeerTable().size() < 2) {
        auto [ip, port] = ni.receiveDataUDP(recvbuf, recvbuflen);
        if (!peerTable.contains(ip)) {
            //TODO: find a way to remove or not add own ip
            addPeer(ip, port);
            if (recvbuf[0] == '0') {
                broadcastResponse(); //TODO: maybe send info directly to requester's udp port
            }
        }
    }
}

void PeerDiscovery::broadcastRequest() const {
    std::string sendbuf = "0" + std::to_string(ni.getTCPServerPort());
    ni.broadcast(sendbuf, static_cast<int>(sendbuf.size()));
}

void PeerDiscovery::broadcastResponse() const {
    std::string sendbuf = "1" + std::to_string(ni.getTCPServerPort());
    ni.broadcast(sendbuf, static_cast<int>(sendbuf.size()));
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
