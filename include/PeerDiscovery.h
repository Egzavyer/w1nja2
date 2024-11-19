#ifndef PEER_H
#define PEER_H

#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

typedef struct NearbyPeer {
    std::string ipAddress;
    int port;
} NearbyPeer;

class PeerDiscovery {
public:
    PeerDiscovery();

    SOCKET getSock() const;

    std::vector<NearbyPeer> getPeerTable();

    void broadcastRequest();

    void broadcastResponse();

    void discoverPeers();


private:
    static void initWinsock();

    static SOCKET createSocket();

    void bindSocket();

    void addPeer(const std::string &ipAddress, const int &port);

    std::vector<NearbyPeer> peerTable;
    SOCKET sock;
    int udpPort = 8080;
    struct sockaddr_in udpAddr{AF_INET, htons(udpPort)};
    struct sockaddr_in peerAddr{};
    struct sockaddr_in broadcastAddr{AF_INET, htons(udpPort)};
};


#endif //PEER_H