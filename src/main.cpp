#include <iostream>
#include "PeerDiscovery.h"
#include "PeerDiscovery.h"
#include <vector>
#include <thread>

int main()
{
    PeerDiscovery peer;
    // peer.broadcastRequest();
    std::thread t(&PeerDiscovery::discoverPeers, &peer);
    // peer.discoverPeers(); // probably have this always running in separate thread to keep polling broadcasted peers
    // while (peer.getPeerTable().empty()){}
    while (peer.getPeerTable().size() < 2)
    {
    }
    const auto table = peer.getPeerTable();
    for (const auto &tablePeer : table)
    {
        std::cout << "Peer at: " << tablePeer.ipAddress << "::" << tablePeer.port << std::endl;
    }
    t.join();
    return 0;
}
