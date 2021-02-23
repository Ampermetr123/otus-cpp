/**
 * @file session.h
 * @author  Sergey
 * @brief Class for connection handler
 */
#pragma once
#include <stdio.h>
#include <string>

#include "asio.h"
#include "iterable_result.h"
#include "optlog.h"
#include "storage.h"


 /**
  * @brief Handle connection class
  */
class Session : public std::enable_shared_from_this<Session>
{
private:
    ba::ip::tcp::socket socket;
    const optlog::OptLog& logger;
    inline static const int max_datainp_lenght = 512;
    char data[max_datainp_lenght];
    std::string tail; // rest of string without \n
    Storage storage;

public:
    Session(ba::ip::tcp::socket sock, const optlog::OptLog& log);
    ~Session();
    static auto create(ba::ip::tcp::socket sock, const optlog::OptLog& log) {
        return std::make_shared<Session>(std::move(sock), log);
    }
    void start();
    void read();
    void process_data_input(char* data, std::size_t length);
    void handle_command(std::string str);
    void write_result(std::unique_ptr<IterableResult>  res);
};