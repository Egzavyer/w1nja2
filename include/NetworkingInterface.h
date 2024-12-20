#ifndef NETWORKING_INTERFACE_H
#define NETWORKING_INTERFACE_H

#include <unordered_map>
#include <string>
#include <cstring>

class NetworkingInterface {
public:
    virtual ~NetworkingInterface() = default;

    virtual void startup() = 0;

    virtual unsigned long long getUDPSocket() = 0;

    virtual unsigned long long getTCPServerSocket() = 0;

    virtual unsigned long long getTCPClientSocket() = 0;

    virtual int getUDPPort() = 0;

    virtual int getTCPServerPort() = 0;

    virtual int getTCPClientPort() = 0;

    virtual std::string &getOwnIp() = 0;

    virtual void setOwnIp() = 0;

    virtual void broadcast(std::string sendbuf, int sendbuflen) = 0;

    virtual std::pair<std::string, int> receiveDataUDP(char *recvbuf, int recvbuflen) = 0;

    virtual void connectToSocket(std::string &ip, int &port) = 0;

    virtual void listenOnSocket() = 0;

    virtual const unsigned long long acceptSocketConnection() = 0;

    virtual int sendDataTCP(const unsigned long long &socket, const char *sendbuf, int sendbuflen) = 0;

    virtual int receiveDataTCP(const unsigned long long &socket, char *recvbuf, int recvbuflen) = 0;

    virtual void initialise() = 0;

    virtual void createUDPSocket() = 0;

    virtual void createTCPServerSocket() = 0;

    virtual void createTCPClientSocket() = 0;

    virtual void bindUDPSocket() = 0;

    virtual void bindTCPServerSocket() = 0;
};

#endif //NETWORKING_INTERFACE_H
