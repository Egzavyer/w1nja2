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
#include <netdb.h>

#include "NetworkingInterface.h"

class LinuxInterface : public NetworkingInterface {
public:
    LinuxInterface();

    ~LinuxInterface() override;

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

    int udpSocket;
    int tcpServerSocket;
    int tcpClientSocket;
    int udpPort;
    int tcpServerPort;
    int tcpClientPort;
    std::string ownIP;
    struct sockaddr_in broadcastAddr{};
};

#endif //__linux__

#endif //LINUX_INTERFACE_H
