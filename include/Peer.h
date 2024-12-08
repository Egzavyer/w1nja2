#ifndef PEER_H
#define PEER_H

#include <thread>

#include "PeerDiscovery.h"
#include "FileHandler.h"

class Peer {
public:
    explicit Peer(PeerDiscovery &peerDiscovery, FileHandler &fileHandler);

    void startPeerDiscovery();

private:
    PeerDiscovery &pd;
    FileHandler &fh;
};


#endif //PEER_H
