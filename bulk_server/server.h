/**
 * @file server.h
 * @author Sergey Simonov
 * @brief Server declaration
 *
 */
#pragma once

#include <cstddef>
#include <iostream>
#include "asio.h"

#include "optlog.h"

class BulkServer {
public:
    BulkServer(uint16_t port, size_t bulk_size, int verbose);
    void run();

private:
    const size_t bulk_size;
    const optlog::OptLog log1;
    
    ba::io_context io_context;
    ba::ip::tcp::endpoint tcp_endpoint;
    ba::ip::tcp::acceptor acceptor;
    void do_accept();
};