/**
 * @file server.cpp
 * @author  Sergey Simonov
 * @brief Server implementation
 */

#include <future>
#include "server.h"
#include "session.h"

/**
 * @brief Construct a new Join Server:: Join Server object
 * @param port tcp port to accept connection
 * @param verbose show log to console
 */
JoinServer::JoinServer(uint16_t port,  int verbose) :
    log1(std::clog, verbose, 1),
    tcp_endpoint(ba::ip::address_v4::any(), port),
    acceptor(io_service, tcp_endpoint) {
    log1 << "Server parameters:"
        "port = " << port << "\n"
        "verbose = " << verbose << "" << std::endl;
}

/**
 * @brief Start server and block until terminate signal
 */
void JoinServer::run() {
    log1 << "Join server started \n";
    // Terminate Handler
    ba::signal_set signals(io_service, SIGINT, SIGTERM);
    signals.async_wait([this](const bs::error_code& errcode,
        [[maybe_unused]] int number) {
            if (!errcode)  io_service.stop();
    });

    do_accept();
    
    // Pool with 2 threads to support parallel read, write and processing
    auto f1 = std::async([this]() {io_service.run();});
    auto f2 = std::async([this]() {io_service.run();});
    f1.get();
    f2.get();
    log1 << "Join server stoped \n";
}

/**
 * @brief Async accept loop. Creates seessions on connections.
 */
void JoinServer::do_accept() 
{
    acceptor.async_accept(
    [this](boost::system::error_code ec, ba::ip::tcp::socket socket)
    {
    if (!ec) {
        Session::create(std::move(socket), log1)->start();
    } else {
        log1 << ec << std::endl;
    }   
    do_accept();
    }
    );
}
