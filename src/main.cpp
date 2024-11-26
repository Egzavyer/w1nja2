#include "PeerDiscovery.h"
#include "WinsockInterface.h"
#include <vector>
#include <thread>
#include <iostream>

int main() {
    WinsockInterface wi;
    wi.startup();
    PeerDiscovery peer(wi);
    std::thread t(&PeerDiscovery::discoverPeers, &peer);
    if (t.joinable())
        t.join();
    for (const auto &peers: peer.getPeerTable()) {
        std::cout << peers.first << "::" << peers.second << std::endl;
    }
    return 0;
}
