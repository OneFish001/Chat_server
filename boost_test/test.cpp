#include <boost/asio.hpp>
#include <iostream>

int main() {
    boost::asio::io_context io;
    boost::asio::steady_timer timer(io, boost::asio::chrono::seconds(1));
    timer.async_wait([](const boost::system::error_code&) {
        std::cout << "Hello Boost!" << std::endl;
    });
    io.run();
    return 0;
}