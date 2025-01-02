#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "NetworkingInterface.h"
#include <filesystem>
#include <iostream>
#include <fstream>


class FileHandler {
public:
    explicit FileHandler(NetworkingInterface &networkingInterface);

    void getAvailableFiles();

    void sendFile(const std::string &filename);

    void receiveFile(const std::string &filename, const int &fileSize);

private:
    NetworkingInterface &ni;
    const std::filesystem::path defaultPath{"C:/dev/cpp/w1nja2/files"};
};
#endif //FILE_HANDLER_H
