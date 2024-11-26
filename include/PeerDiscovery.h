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

    void broadcastRequest();

    void broadcastResponse();

    void discoverPeers();


private:

    void addPeer(const std::string &ipAddress, const int &port);

    std::mutex peerTableMutex;
    std::unordered_map<std::string, int> peerTable;
    NetworkingInterface &n;

    int udpPort = 8080;

    //struct sockaddr_in peerAddr{};
    //struct sockaddr_in broadcastAddr{AF_INET, htons(udpPort)};
};


#endif //PEER_H
