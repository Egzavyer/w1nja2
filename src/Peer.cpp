#include "../include/Peer.h"

Peer::Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &connectionHandler,
           FileHandler &fileHandler) : pd(peerDiscovery),
                                       ch(connectionHandler), fh(fileHandler) {
}

void Peer::startPeer() {
    startPeerDiscovery();
    startHandlingConnections();
}

void Peer::startPeerDiscovery() {
    std::thread t(&PeerDiscovery::discoverPeers, &pd);
    t.detach();
}

void Peer::startHandlingConnections() {
    std::thread t(&ConnectionHandler::checkForConnections, &ch);
    t.detach();
}

void Peer::connectToPeer(std::string &ip) {
    //TODO: breaks everything since trying to get a value from table that might not be there yet
    while (pd.getPeerTable().empty() || !pd.getPeerTable().contains(ip)) {
    }
    std::cout << "Connecting to :" << ip << "::" << pd.getPeerTable()[ip] << std::endl;
    ch.connectTo(ip, pd.getPeerTable()[ip]);
}

std::unordered_map<int, std::string> Peer::choosePeer() {
    std::unordered_map<int, std::string> peerMap;
    int i{0};
    while (pd.getPeerTable().empty()) {
    }
    std::cout << "----------------------------\n";
    for (const auto &[fst, snd]: pd.getPeerTable()) {
        peerMap[i] = fst;
        std::cout << "|   [" << i << "] : " << fst << "   |\n";
        i++;
    }
    std::cout << "----------------------------\n";
    return peerMap;
}



