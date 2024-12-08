#ifndef PEER_DISCOVERY_H
#define PEER_DISCOVERY_H

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <mutex>

#include "NetworkingInterface.h"

class PeerDiscovery {
public:
    explicit PeerDiscovery(NetworkingInterface &networkingInterface);

    std::unordered_map<std::string, int> getPeerTable();

    void discoverPeers();

private:
    void addPeer(const std::string &ipAddress, const int &port);

    void broadcastRequest() const;

    void broadcastResponse() const;

    std::mutex peerTableMutex;
    std::unordered_map<std::string, int> peerTable;
    NetworkingInterface &ni;
};


#endif //PEER_DISCOVERY_H
