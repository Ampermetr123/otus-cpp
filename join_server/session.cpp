/**
 * @file session.cpp
 * @author Sergey Simonov
 * @brief JoinServer session implementation
 */

#include "session.h"
#include "commands.h"

 /**
  * @brief Construct a new Session:: Session object
  * @param sock connected socket
  * @param log looger
  */
Session::Session(ba::ip::tcp::socket sock, const optlog::OptLog& log) :
    socket(std::move(sock)),
    logger(log),
    storage("A", "B") {
    logger << "Conneced with " << socket.remote_endpoint().address().to_string() << std::endl;
}

Session::~Session() {
    logger << "Connection closed with " << socket.remote_endpoint().address().to_string() << std::endl;
}

/**
 * @brief Enter point
 */
void Session::start() {
    read();
}

/**
 * @brief Read from tcp socket
 */
void Session::read() {
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
 * @brief Processing raw data to lines
 */
void Session::process_data_input(char* data, std::size_t length) {
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

/**
 * @brief Handle client command (string)
 * @param str string with command
 */
void Session::handle_command(std::string str) {
    auto [cmd, errstr] = make_command(storage, std::move(str));
    if (!cmd) {
        write_result(std::make_unique<StringResult>(errstr));
    }
    else {
        boost::asio::io_context& io_context = static_cast<boost::asio::io_context&>(socket.get_executor().context());
        std::shared_ptr<ICommand> shared_cmd = std::move(cmd);
        io_context.post([cmd_ = shared_cmd, this, self = shared_from_this()]()
        {
            write_result(cmd_->execute());
        }
        );
    }
}

/**
 * @brief Send asynchrounosuly all strings in IterResult to client
 *
 * @param res Result with strings
 */
void Session::write_result(std::unique_ptr<IterableResult>  res) {
    if (!res) return;
    for (auto it = res->begin(); it != res->end(); ++it) {
        auto message = std::make_shared<std::string>(*it + "\n");
        logger << "> " << *message;
        ba::async_write(socket, ba::buffer(*message),
            [shared = message, this, self = shared_from_this()](boost::system::error_code ec, std::size_t) {
            if (ec) logger << ec << std::endl;
        });
    };
}


