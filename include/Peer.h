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

private:
    void startPeerDiscovery();

    void startHandlingConnections();

    PeerDiscovery &pd;
    ConnectionHandler &ch;
};


#endif //PEER_H
