#include "PeerDiscovery.h"
#include "WinsockInterface.h"
#include "LinuxInterface.h"
#include <thread>
#include "Peer.h"
#include "ConnectionHandler.h"


int main() {
#ifdef _WIN32
    WinsockInterface wi;
    wi.startup();
    PeerDiscovery peerDiscovery(wi);
    ConnectionHandler fileHandler(wi);
#elif __linux__
    LinuxInterface li;
    li.startup();
    PeerDiscovery peerDiscovery(li);
    ConnectionHandler fileHandler(li);
#endif //_Win32

    Peer peer(peerDiscovery, fileHandler);
    std::thread t(&Peer::startPeer,peer);
    if (t.joinable()) {
        t.join();
    }
    return 0;
}
