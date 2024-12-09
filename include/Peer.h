#ifndef PEER_H
#define PEER_H

#include <thread>

#include "PeerDiscovery.h"
#include "ConnectionHandler.h"

class Peer {
public:
    explicit Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &fileHandler);

    void startPeerDiscovery();

private:
    PeerDiscovery &pd;
    ConnectionHandler &fh;
};


#endif //PEER_H
