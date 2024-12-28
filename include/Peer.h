#ifndef PEER_H
#define PEER_H

#include <FileHandler.h>
#include <thread>

#include "PeerDiscovery.h"
#include "ConnectionHandler.h"

class Peer {
public:
    explicit Peer(PeerDiscovery &peerDiscovery, ConnectionHandler &connectionHandler, FileHandler &fileHandler);

    void startPeer();

    void connectToPeer(std::string &ip);

    std::unordered_map<int, std::string> choosePeer();

private:
    void startPeerDiscovery();

    void startHandlingConnections();

    PeerDiscovery &pd;
    ConnectionHandler &ch;
    FileHandler &fh;
};


#endif //PEER_H
