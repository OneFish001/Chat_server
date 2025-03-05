#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <memory>
#include <thread>

#include "../db_adapter/ConnectionPool.h"
#include "../log/logger.h"

using namespace boost::asio;
using namespace boost::asio::ip;


class ChatServer {
    // ConnectionPool db_pool_;//新增加连接池

public:
    ChatServer(io_context& io_context,short port,int thread_pool_size);
    void start();
    // void handleMessage(const std::string&meg);

private:
   void start_accept();
   void handle_accept(std::shared_ptr<tcp::socket> socket,const boost::system::error_code& error);

   void start_read(std::shared_ptr<tcp::socket> socket);
   void handle_read(std::shared_ptr<tcp::socket> socket,std::shared_ptr<std::vector<char> > buffer,const boost::system::error_code& error,size_t bytes);
   void boardcast(const std::string& message);

   io_context& io_context_;
   tcp::acceptor acceptor_;
   std::vector<std::shared_ptr <tcp::socket> > clients;
   std::mutex clients_mutex_; //保护clients_的互斥锁
   int thread_pool_size_;//线程池大小



};