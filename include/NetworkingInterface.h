#ifndef NETWORKING_INTERFACE_H
#define NETWORKING_INTERFACE_H

#include <unordered_map>
#include <string>

class NetworkingInterface {
public:
    virtual ~NetworkingInterface() = default;

    virtual void startup() = 0;

    virtual unsigned long long getUDPSocket() = 0;

    virtual unsigned long long getTCPSocket() = 0;

    virtual int getUDPPort() = 0;

    virtual int getTCPPort() = 0;

    virtual void broadcast(std::string sendbuf, int sendbuflen) = 0;

    virtual std::pair<std::string, int> receiveDataUDP(char *recvbuf, int recvbuflen) = 0;

    virtual void initialise() = 0;

    virtual void createUDPSocket() = 0;

    virtual void createTCPSocket() = 0;

    virtual void bindUDPSocket() = 0;

    virtual void bindTCPSocket() = 0;
};

#endif //NETWORKING_INTERFACE_H
