#include "PeerDiscovery.h"
#include "WinsockInterface.h"
#include "LinuxInterface.h"
#include <thread>
#include "Peer.h"
#include "ConnectionHandler.h"
#include <cctype>
#include "FileHandler.h"


int main() {
#ifdef _WIN32
    WinsockInterface wi;
    wi.startup();
    PeerDiscovery peerDiscovery(wi);
    ConnectionHandler connectionHandler(wi);
    FileHandler fileHandler(wi);

#elif __linux__
    LinuxInterface li;
    li.startup();
    PeerDiscovery peerDiscovery(li);
    ConnectionHandler connectionHandler(li);
    FileHandler fileHandler(li);
#endif //_Win32

    Peer peer(peerDiscovery, connectionHandler, fileHandler);
    peer.startPeer();
    /*std::thread t(&Peer::startPeer,peer);
    if (t.joinable()) {
        t.join();
    }*/
    bool wait = true;
    std::string peerChoice;
    std::string ip;
    while (wait) {
        //print peer table and allow user to choose peer or refresh
        std::unordered_map<int, std::string> peerMap = peer.choosePeer();
        std::cout << "Choose a peer to connect to or press \'r\' to refresh: ";
        std::getline(std::cin, peerChoice);
        if (peerChoice != "r" && isdigit(peerChoice.c_str()[0])) {
            ip = peerMap[std::stoi(peerChoice)];
            wait = false;
        }
    }
    peer.connectToPeer(ip);

    fileHandler.getAvailableFiles();
    fileHandler.sendFile("test.txt");
    while (true) {
        //TODO: find a way to keep it running
    }
    return 0;
}
