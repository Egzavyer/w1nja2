set(SOURCES
        main.cpp
        PeerDiscovery.cpp
)

add_executable(w1nja ${SOURCES})
if (WIN32)
    target_link_libraries(w1nja PRIVATE ws2_32)
endif (WIN32)