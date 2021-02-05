/**
 * @file server.cpp
 * @author  Sergey Simonov
 * @brief Server implementation
 */
#include "server.h"
#include "session.h"

BulkServer::BulkServer(uint16_t port, size_t bulk_size, int verbose) :
    bulk_size(bulk_size),
    log1(std::clog, verbose, 1),
    tcp_endpoint(ba::ip::address_v4::any(), port),
    acceptor(io_context, tcp_endpoint) {
    log1 << "Server parameters:"
        "port = " << port << "\n"
        "bulk_size = " << bulk_size << "\n"
        "verbose = " << verbose << "" << std::endl;
}

/**
 * @brief Start server until terminate signal
 */
void BulkServer::run() {
    log1 << "Server started \n";
    
    // Terminate Handler
    ba::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait(
        [this](const bs::error_code& errcode, [[maybe_unused]] int number) {
            if (!errcode)
                io_context.stop();
        }
    );

    // Accept and process...
    do_accept();
    io_context.run();
    
    log1 << "Server stoped \n";
}

/**
 * @brief Async accept loop. Creates seessions on connections.
 */
void BulkServer::do_accept() 
{
    acceptor.async_accept(
        [this] (boost::system::error_code ec, ba::ip::tcp::socket socket){ 
        if (!ec) {
            auto s = Session::create(std::move(socket), bulk_size, log1);
            s->start();
        } else {
            log1 << ec << std::endl;
        }   
        
        do_accept();
        });
}
