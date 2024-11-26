#ifndef PEER_H
#define PEER_H

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <mutex>

#include "NetworkingInterface.h"

class PeerDiscovery {
public:
    explicit PeerDiscovery(NetworkingInterface &ni);

    std::unordered_map<std::string, int> getPeerTable();

    void discoverPeers();


private:

    void addPeer(const std::string &ipAddress, const int &port);

    void broadcastRequest();

    void broadcastResponse();

    std::mutex peerTableMutex;
    std::unordered_map<std::string, int> peerTable;
    NetworkingInterface &n;
};


#endif //PEER_H
