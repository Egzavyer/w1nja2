#ifndef LINUX_INTERFACE_H
#define LINUX_INTERFACE_H

#ifdef __linux__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <iostream>
#include <sys/types.h>

#include "NetworkingInterface.h"

class LinuxInterface : public NetworkingInterface {
public:
    LinuxInterface();

    ~LinuxInterface() override;

    void startup() override;

    unsigned long long getUDPSocket() override;

    unsigned long long getTCPSocket() override;

    int getUDPPort() override;

    int getTCPPort() override;

    std::pair<std::string, int> receiveDataUDP(char *recvbuf, int recvbuflen) override;

    void broadcast(std::string sendbuf, int sendbuflen) override;

    void connectToSocket(std::string &ip, int &port) override;

private:
    void initialise() override;

    void createUDPSocket() override;

    void createTCPSocket() override;

    void bindUDPSocket() override;

    void bindTCPSocket() override;

    int udpSocket;
    int tcpSocket;
    int udpPort;
    int tcpPort;
    struct sockaddr_in broadcastAddr{};
};

#endif //__linux__

#endif //LINUX_INTERFACE_H
