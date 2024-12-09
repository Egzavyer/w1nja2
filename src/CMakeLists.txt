if (WIN32)
    set(SOURCES
            main.cpp
            PeerDiscovery.cpp
            WinsockInterface.cpp
            Peer.cpp
            ConnectionHandler.cpp
    )
elseif (UNIX)
    set(SOURCES
            main.cpp
            PeerDiscovery.cpp
            LinuxInterface.cpp
            Peer.cpp
            ConnectionHandler.cpp
    )
endif (WIN32)

add_executable(w1nja ${SOURCES})
if (WIN32)
    target_link_libraries(w1nja PRIVATE ws2_32)
endif (WIN32)