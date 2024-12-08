#include "PeerDiscovery.h"
#include "WinsockInterface.h"
#include "LinuxInterface.h"
#include <thread>
#include "Peer.h"
#include "FileHandler.h"


int main() {
#ifdef _WIN32
    WinsockInterface wi;
    wi.startup();
    PeerDiscovery peerDiscovery(wi);
    FileHandler fileHandler(wi);
#elif __linux__
    LinuxInterface li;
    li.startup();
    PeerDiscovery peerDiscovery(li);
    FileHandler fileHandler(li);
#endif //_Win32

    Peer peer(peerDiscovery, fileHandler);
    peer.startPeerDiscovery();
    return 0;
}
