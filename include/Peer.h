#ifndef PEER_H
#define PEER_H

#include <thread>

#include "PeerDiscovery.h"
#include "ConnectionHandler.h"

class Peer {
public:
    explicit Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &fileHandler);

    void startPeer();

private:
    void startPeerDiscovery();

    PeerDiscovery &pd;
    ConnectionHandler &fh;
};


#endif //PEER_H
