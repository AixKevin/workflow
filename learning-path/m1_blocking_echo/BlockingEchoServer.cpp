#include "BlockingEchoServer.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

BlockingEchoServer::BlockingEchoServer(const char* ip, uint16_t port)
    : ip_(ip), port_(port), listen_fd_(-1), client_fd_(-1) {}

BlockingEchoServer::~BlockingEchoServer() {
    stop();
}

bool BlockingEchoServer::init() {
    // TODO(M1):
    // 1) socket(AF_INET, SOCK_STREAM, 0)
    // 2) setsockopt(SO_REUSEADDR)
    // 3) bind()
    // 4) listen()
    // Return true on success.
    return false;
}

void BlockingEchoServer::run() {
    // TODO(M1):
    // 1) accept() one client
    // 2) loop: read() -> write() back
    // 3) client close when read returns 0
    // 4) handle EINTR and common errors
}

void BlockingEchoServer::stop() {
    if (client_fd_ >= 0) {
        close(client_fd_);
        client_fd_ = -1;
    }

    if (listen_fd_ >= 0) {
        close(listen_fd_);
        listen_fd_ = -1;
    }
}
