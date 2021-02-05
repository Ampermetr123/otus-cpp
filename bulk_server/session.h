/**
 * @file session.h
 * @author  Sergey
 * @brief Class for connection handler
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include "asio.h"

#include "server.h"
#include "optlog.h"
#include "async.h"

 /**
  * @brief Handle connection in BulkServer
  *
  */
class Session : public std::enable_shared_from_this<Session>
{
private:
    ba::ip::tcp::socket socket;

    async::handle_t async_handle;
    const optlog::OptLog& logger;

    inline static const int max_data_lenght = 512;
    char data[max_data_lenght];
    std::string tail; // rest of string without \n

public:
    Session(ba::ip::tcp::socket sock, size_t bulk_size, const optlog::OptLog& log) :
        socket(std::move(sock)),
        logger(log) {
        async_handle = async::connect(bulk_size);
        logger << "Conneced with " << socket.remote_endpoint().address().to_string() << std::endl;
    }


    ~Session() {
        if (tail.size()) { // End of seession is like EOF
            async::receive(async_handle, tail.c_str(), tail.size());
        }
        async::disconnect(async_handle);
        logger << "Connection closed with " << socket.remote_endpoint().address().to_string() << std::endl;
    }


    static auto create(ba::ip::tcp::socket sock, size_t bulk_size, const optlog::OptLog& log) {
        return std::make_shared<Session>(std::move(sock), bulk_size, log);
    }

    /**
     * @brief Enter point
     *
     */
    void start() {
        read();
    }

    /**
     * @brief Read and process incoming data
     *
     */
    void read() {
        auto self(shared_from_this());
        socket.async_read_some(ba::buffer(data, max_data_lenght),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec && length > 0) {
                    logger << " [ received " << length << " bytes ]" << std::endl;
                    process(data, length);
                    read();
                }
                else {
                    logger << ec << std::endl;
                }
            }
        );
    }

    /**
     * @brief Processing of streamed data
     *
     * @param data
     * @param length
     */
    void process(char* data, std::size_t length) {
        char sep = '\n';
        auto end = data + length;
        auto beg = data;
        if (*beg == sep && tail.size()) {
            async::receive(async_handle, tail.c_str(), tail.size());
            tail.clear();
        };
        while (beg < end && *beg == sep) ++beg;
        auto it = beg;
        while (it < end && *it != sep) ++it;
        while (it < end) {
            if (tail.size()) {
                tail.append(beg, it);
                async::receive(async_handle, tail.c_str(), tail.size());
                tail.clear();
            }
            else {
                async::receive(async_handle, beg, it - beg);
            }
            beg = it;
            while (beg < end && *beg == sep) ++beg;
            it = beg;
            while (it < end && *it != sep) ++it;
        }
        if (beg != end && beg != it) {
            tail = std::string(beg, end - beg);

        }
    }

};