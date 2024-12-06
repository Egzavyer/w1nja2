#include "PeerDiscovery.h"
#include "WinsockInterface.h"
#include "LinuxInterface.h"
#include <vector>
#include <thread>

int main() {
#ifdef _WIN32
    WinsockInterface wi;
    wi.startup();
    PeerDiscovery peer(wi);
#elif __linux__
    LinuxInterface li;
    li.startup();
    PeerDiscovery peer(li);
#endif //_Win32

    std::thread t(&PeerDiscovery::discoverPeers, &peer);
    if (t.joinable())
        t.join();
    return 0;
}
