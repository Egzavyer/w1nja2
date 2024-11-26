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

    int getUDPPort() override;

    int getTCPPort() override;

    std::pair<std::string, int> receiveDataUDP(char *recvbuf, int recvbuflen) override;

    void broadcast(std::string sendbuf, int sendbuflen) override;


private:
    void initialise() override;

    void createUDPSocket() override;

    void createTCPSocket() override;

    void bindUDPSocket() override;

    void bindTCPSocket() override;

    SOCKET udpSocket;
    SOCKET tcpSocket;
    int udpPort;
    int tcpPort;
    struct sockaddr_in broadcastAddr{};

};


#endif //WINSOCK_INTERFACE_H
