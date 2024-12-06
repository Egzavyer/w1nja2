if (WIN32)
    set(SOURCES
            main.cpp
            PeerDiscovery.cpp
            WinsockInterface.cpp
    )
elseif (UNIX)
    set(SOURCES
            main.cpp
            PeerDiscovery.cpp
            LinuxInterface.cpp
    )
endif (WIN32)

add_executable(w1nja ${SOURCES})
if (WIN32)
    target_link_libraries(w1nja PRIVATE ws2_32)
endif (WIN32)