#ifndef WINSOCK_INTERFACE_H
#define WINSOCK_INTERFACE_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <stdexcept>
#include <iostream>

#include "NetworkingInterface.h"

class WinsockInterface : public NetworkingInterface {
public:
    WinsockInterface();

    void startup() override;

    unsigned long long getUDPSocket() override;

    unsigned long long getTCPSocket() override;

    std::string receiveDataUDP(char *recvbuf, int recvbuflen) override;

    void broadcastRequest() override;

    void broadcastResponse() override;

private:
    void initialise() override;

    void createUDPSocket() override;

    void createTCPSocket() override;

    void bindUDPSocket() override;

    void bindTCPSocket() override;

    unsigned long long udpSocket;
    unsigned long long tcpSocket;
    int udpPort;
    int tcpPort;
    struct sockaddr_in broadcastAddr{AF_INET, htons(udpPort)};

};


#endif //WINSOCK_INTERFACE_H
