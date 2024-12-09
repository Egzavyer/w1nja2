#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "NetworkingInterface.h"

class ConnectionHandler {
public:
    explicit ConnectionHandler(NetworkingInterface &networkingInterface);

private:
    NetworkingInterface &ni;
};


#endif //FILE_HANDLER_H
