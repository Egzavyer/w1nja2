#ifndef WINSOCK_INTERFACE_H
#define WINSOCK_INTERFACE_H

#ifdef _WIN32

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

    unsigned long long getTCPServerSocket() override;

    unsigned long long getTCPClientSocket() override;

    int getUDPPort() override;

    int getTCPServerPort() override;

    int getTCPClientPort() override;

    std::string &getOwnIp() override;

    std::pair<std::string, int> receiveDataUDP(char *recvbuf, int recvbuflen) override;

    void broadcast(std::string sendbuf, int sendbuflen) override;

    void connectToSocket(std::string &ip, int &port) override;

    const unsigned long long acceptSocketConnection() override;

    int sendDataTCP(const unsigned long long &socket, const char *sendbuf, int sendbuflen) override;

    int receiveDataTCP(const unsigned long long &socket, char *recvbuf, int recvbuflen) override;

private:
    void initialise() override;

    void createUDPSocket() override;

    void createTCPServerSocket() override;

    void createTCPClientSocket() override;

    void bindUDPSocket() override;

    void bindTCPServerSocket() override;

    void listenOnSocket() override;

    void setOwnIp() override;

    SOCKET udpSocket;
    SOCKET tcpServerSocket;
    SOCKET tcpClientSocket;
    int udpPort;
    int tcpServerPort;
    int tcpClientPort;
    std::string ownIP;
    struct sockaddr_in broadcastAddr{};
};

#endif //_WIN32
#endif //WINSOCK_INTERFACE_H
