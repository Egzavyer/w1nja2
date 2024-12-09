#include "../include/Peer.h"

Peer::Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &fileHandler) : pd(peerDiscovery), fh(fileHandler) {
}

void Peer::startPeerDiscovery() {
    std::thread t(&PeerDiscovery::discoverPeers, &pd);
    if (t.joinable())
        t.join();
}
