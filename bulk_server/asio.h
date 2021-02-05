
/**
 * @brief Helper for declaration and includes for using boost:asio 
 * 
 */
#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

namespace ba = boost::asio;
namespace bs = boost::system;

inline std::ostream& operator<<(std::ostream& os, bs::error_code ec) {
    os << ec.category().name() << " : " << ec.value() << " : " << ec.message();
    return os;
}