#include "ChatServer.h"
//#include "include/ChatServer.h"


ChatServer::ChatServer(io_context& io_context,short port):
io_context_(io_context),acceptor_(io_context,tcp::endpoint(tcp::v4(),port)){
    std::cout<<"服务器启动，开始监听端口："<<port<<"\n";
    start();

}

void ChatServer::start(){
    start_accept();
}

void ChatServer::start_accept(){
    //创建一个新的socket对象
    auto socket=std::make_shared<tcp::socket>(io_context_);

    //异步连接
    acceptor_.async_accept(*socket,[this,socket](const boost::system::error_code& error){
        handle_accept(socket,error);
    });

}

void ChatServer::handle_accept(std::shared_ptr<tcp::socket> socket,const boost::system::error_code& error){
    if(!error){
        std::cout<<"有新的客户端连接："<<socket->remote_endpoint()<<"\n";

        //将socket存入客户端列表
        clients.push_back(socket);

        //开始读取消息
        start_read(socket);

        //继续等待下一个连接，循环
        start_accept();}
    else{
        std::cerr<<"连接错误！！！错误信息："<<error.message()<<"\n";
    }


    }

void ChatServer::start_read(std::shared_ptr<tcp::socket> socket){
    auto buffer=std::make_shared<std::vector<char>>(1024);

    //异步读取数据
    socket->async_read_some(boost::asio::buffer(*buffer),[this,socket,buffer](const boost::system::error_code& error,size_t bytes){
        handle_read(socket,buffer,error,bytes);

    });
}

void ChatServer::handle_read(std::shared_ptr<tcp::socket> socket,std::shared_ptr<std::vector<char>> buffer,const boost::system::error_code& error,size_t bytes){
    if(!error && bytes>0){
        std::string message(buffer->begin(),buffer->begin()+bytes);
        std::cout<<"收到消息："<<message<<"\n";

        //广播消息
        boardcast(message);
        //继续读取
        start_read(socket);

    }else{
        std::cout<<"读取错误！客户端断开："<<socket->remote_endpoint()<<"\n";

        //从客户端列表移除
        auto it=std::find(clients.begin(),clients.end(),socket);
        if(it!=clients.end()){
            clients.erase(it);
        }
    }

}

void ChatServer::boardcast(const std::string& message){
    for(auto &client : clients){
        boost::asio::async_write(*client,boost::asio::buffer(message+"\n"),[](const boost::system::error_code&,size_t){});

    }
}



