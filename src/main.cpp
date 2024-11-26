#include <iostream>
#include "PeerDiscovery.h"
#include "WinsockInterface.h"
#include "NetworkingInterface.h"
#include <vector>
#include <thread>

int main() {
    WinsockInterface wi;
    wi.startup();
    PeerDiscovery peer(wi);
    // peer.broadcastRequest();
    std::thread t(&PeerDiscovery::discoverPeers, &peer);
    // peer.discoverPeers(); // probably have this always running in separate thread to keep polling broadcasted peers
    // while (peer.getPeerTable().empty()){}
    if (t.joinable())
        t.join();
    return 0;
}
