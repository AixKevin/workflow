#pragma once

#include <cstdint>

class BlockingEchoServer {
public:
    BlockingEchoServer(const char* ip, uint16_t port);
    ~BlockingEchoServer();

    // Step 1: create socket + bind + listen.
    bool init();

    // Step 2: accept one client and do echo loop.
    void run();

    // Step 3: close all resources.
    void stop();

private:
    const char* ip_;
    uint16_t port_;
    int listen_fd_;
    int client_fd_;
};
