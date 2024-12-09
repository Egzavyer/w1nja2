#include "../include/Peer.h"

Peer::Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &fileHandler) : pd(peerDiscovery), fh(fileHandler) {
}

void Peer::startPeer() {
    startPeerDiscovery();
    while (true) {
        //TODO: find a way to keep in running
    }
}

void Peer::startPeerDiscovery() {
    std::thread t(&PeerDiscovery::discoverPeers, &pd);
    t.detach();
}
