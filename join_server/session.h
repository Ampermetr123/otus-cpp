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
#include <queue>
#include "asio.h"

#include "server.h"
#include "optlog.h"
#include "commands.h"
#include "storage.h"


 /**
  * @brief Handle connection
  *
  */
class Session : public std::enable_shared_from_this<Session>
{
private:
    ba::ip::tcp::socket socket;
    const optlog::OptLog& logger;
    inline static const int max_datainp_lenght = 512;
    char data[max_datainp_lenght];
    std::string send_message;
    std::string tail; // rest of string without \n

    //std::queue <std::unique_ptr<joinstorage::ICommand> > cmd_queue;

    std::unique_ptr<IterableResult> send_result;
    ResultIterator send_iter;
    std::string send_value;
    boost::asio::deadline_timer send_timer;
    



    joinstorage::Storage storage;

public:
    Session(ba::ip::tcp::socket sock, const optlog::OptLog& log) :
        socket(std::move(sock)),
        logger(log),
        send_timer(sock.get_executor())
    {
        using namespace boost::posix_time;
        logger << "Conneced with " << socket.remote_endpoint().address().to_string() << std::endl;
        send_timer.expires_from_now(seconds(0));    
    }


    ~Session() {
        if (tail.size()) { // End of seession is like EOF
            handle_command(tail);
        }

        logger << "Connection closed with " << socket.remote_endpoint().address().to_string() << std::endl;
    }


    static auto create(ba::ip::tcp::socket sock, const optlog::OptLog& log) {
        return std::make_shared<Session>(std::move(sock), log);
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
        //auto self(shared_from_this());
        socket.async_read_some(ba::buffer(data, max_datainp_lenght),
            [this, self = shared_from_this()](boost::system::error_code ec, std::size_t length) {
            if (!ec && length > 0) {
                logger << " [ received " << length << " bytes ]" << std::endl;
                process_data_input(data, length);
                read();
            }
            else {
                logger << ec << std::endl;
            }
        }
        );
    }


    /**
     * @brief Processing of streamed data by lines
     *
     * @param data
     * @param length
     */
    void process_data_input(char* data, std::size_t length) {
        const char sep = '\n';
        auto end = data + length;
        auto beg = data;
        if (*beg == sep && tail.size()) {
            handle_command(tail);
            tail.clear();
        };
        while (beg < end && *beg == sep) ++beg;
        auto it = beg;
        while (it < end && *it != sep) ++it;
        while (it < end) {
            if (tail.size()) {
                tail.append(beg, it);
                handle_command(tail);
                tail.clear();
            }
            else {
                handle_command(std::string(beg, it - beg));
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

    void handle_command(std::string str) {
        auto [cmd, errstr] = joinstorage::make_command(storage, std::move(str));
        if (!cmd) {
            //write_message(errstr);
            write_result(std::make_unique<StringResult>(errstr));
        }
        else {
            //cmd_queue.push(std::move(cmd));
            //logger << "+cmd" <<std::endl;
            write_result(cmd->execute());
        }
    }

    // void execute_command(){
    //     if (cmd_queue.empty()){ 
    //         return;
    //     }

    // }


    void write_result(std::unique_ptr<IterableResult>  res) {
        logger<<"*write_result"<<std::endl;
        if (!res) return;
        // wait for previous write_result operation will finish
        // So - we can read and process next comand while sending result. 
        boost::system::error_code ec;
        send_timer.wait(ec);
        // this will protext us to start new write result
        send_timer.expires_from_now(boost::posix_time::pos_infin);
        send_result = std::move(res);
        send_iter = send_result->begin();
        write_iter();
    };


    void write_iter() {
        logger<<"*write_iter"<<std::endl;
        if (!(send_iter != send_result->end())) {
            send_timer.cancel();
            return;
        }
        send_message = *send_iter;
        send_message.append("\n");
        ba::async_write(socket, ba::buffer(send_message),
            [this, self = shared_from_this()](boost::system::error_code ec, std::size_t) {

                if (ec) {
                    send_timer.cancel();
                    logger << ec << std::endl;
                }
                else {
                     logger<<"*write_iter next"<<std::endl;
                    ++send_iter;
                    write_iter();
                }
            }
        );

    }


    void write_message(std::string message) {
        send_message = std::move(message);
        ba::async_write(socket, ba::buffer(message),
            [this, self = shared_from_this()](boost::system::error_code ec, std::size_t) {
            if (ec) {
                logger << ec << std::endl;
            }
        }
        );
    }


};