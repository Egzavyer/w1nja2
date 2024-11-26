#include "PeerDiscovery.h"
#include "WinsockInterface.h"
#include <vector>
#include <thread>

int main() {
    WinsockInterface wi;
    wi.startup();
    PeerDiscovery peer(wi);
    std::thread t(&PeerDiscovery::discoverPeers, &peer);
    if (t.joinable())
        t.join();
    return 0;
}
