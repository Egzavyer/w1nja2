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
    //std::string test = "127.0.0.1";
    std::string test = "172.18.0.3";
    int port = 8081;
    //ch.connectTo(test, port);
    //TODO: breaks everything since trying to get a value from table that might not be there yet
    std::cout << "Connecting to :" << ip << "::" << pd.getPeerTable()[ip] << std::endl;
    while (pd.getPeerTable().size() != 2) {
    }
    ch.connectTo(ip, pd.getPeerTable()[ip]);
}


