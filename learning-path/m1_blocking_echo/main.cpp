#include "BlockingEchoServer.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>\n";
        return 1;
    }

    const char* ip = argv[1];
    int port = std::atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port\n";
        return 1;
    }

    BlockingEchoServer server(ip, static_cast<uint16_t>(port));
    if (!server.init()) {
        std::cerr << "init failed\n";
        return 2;
    }

    server.run();
    return 0;
}
