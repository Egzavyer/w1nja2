#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "NetworkingInterface.h"

class FileHandler {
public:
    explicit FileHandler(NetworkingInterface &networkingInterface);

private:
    NetworkingInterface &ni;
};


#endif //FILE_HANDLER_H
