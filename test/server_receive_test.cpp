#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main() {
    try {
        io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8888));
        tcp::socket socket(io);
        
        std::cout << "Waiting for connection..." << std::endl;
        acceptor.accept(socket);
        std::cout << "Client connected!" << std::endl;
        
        char data[1024];
        while (true) {
            boost::system::error_code error;
            size_t length = socket.read_some(buffer(data), error);
            
            if (error == error::eof) {
                std::cout << "Client disconnected" << std::endl;
                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }

            std::cout << "Received: " << std::string(data, length) << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
