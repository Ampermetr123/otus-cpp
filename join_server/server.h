/**
 * @file server.h
 * @author Sergey Simonov
 * @brief Server declaration
 *
 */
#pragma once

#include "asio.h"
#include "optlog.h"

class JoinServer {
public:
    JoinServer(uint16_t port, int verbose);
    void run();
private:
    const optlog::OptLog log1;
    ba::io_service io_service;
    ba::ip::tcp::endpoint tcp_endpoint;
    ba::ip::tcp::acceptor acceptor;
    void do_accept();
};
