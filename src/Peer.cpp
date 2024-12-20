#include "../include/Peer.h"

Peer::Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &connectionHandler) : pd(peerDiscovery),
    ch(connectionHandler) {
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
    while (pd.getPeerTable().size() < 1 || !pd.getPeerTable().contains(ip)) {
    }
    std::cout << "Connecting to :" << ip << "::" << pd.getPeerTable()[ip] << std::endl;
    ch.connectTo(ip, pd.getPeerTable()[ip]);
}


