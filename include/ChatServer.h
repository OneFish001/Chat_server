#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;


class ChatServer {
public:
    ChatServer(io_context& io_context,short port);
    void start();

private:
   void start_accept();
   void handle_accept(std::shared_ptr<tcp::socket> socket,const boost::system::error_code& error);

   void start_read(std::shared_ptr<tcp::socket> socket);
   void handle_read(std::shared_ptr<tcp::socket> socket,std::shared_ptr<std::vector<char> > buffer,const boost::system::error_code& error,size_t bytes);
   void boardcast(const std::string& message);

   io_context& io_context_;
   tcp::acceptor acceptor_;
   std::vector<std::shared_ptr <tcp::socket> > clients;




};