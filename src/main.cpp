#include <iostream>
#include "PeerDiscovery.h"
#include "PeerDiscovery.h"
#include <vector>

int main() {
    PeerDiscovery peer;
    peer.discoverPeers(); // probably have this always running in separate thread to keep polling broadcasted peers
    const auto table = peer.getPeerTable();
    for (const auto &tablePeer: table) {
        std::cout << "Peer at: " << tablePeer.ipAddress << "::" << tablePeer.port << std::endl;
    }
    return 0;
}
