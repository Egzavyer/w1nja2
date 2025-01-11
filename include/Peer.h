#ifndef PEER_H
#define PEER_H

#include <thread>

#include "PeerDiscovery.h"
#include "ConnectionHandler.h"

class Peer {
public:
    explicit Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &connectionHandler);

    void startPeer();

    void connectToPeer(std::string &ip);

    std::unordered_map<int, std::string> choosePeer();

private:
    void startPeerDiscovery();

    void startHandlingConnections();

    PeerDiscovery &pd;
    ConnectionHandler &ch;
};


#endif //PEER_H
