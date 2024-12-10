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
    std::string test = "127.0.0.1";
    int port = 8081;
    ch.connectTo(test, port);
    //ch.connectTo(ip, pd.getPeerTable()[ip]);
}


