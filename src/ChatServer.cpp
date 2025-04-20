#include "../include/ChatServer.h"
#include "log/logger.h"



ChatServer::ChatServer(io_context& io_context,short port,int thread_pool_size):
io_context_(io_context),acceptor_(io_context,tcp::endpoint(tcp::v4(),port)),thread_pool_size_(thread_pool_size){
    // initLogger();
    LOG_INFO<<"服务器启动，开始监听端口"<<port;
    start();

}

void ChatServer::start(){
    start_accept();
}



void ChatServer::start_accept(){
    //创建一个新的socket对象
    auto socket=std::make_shared<tcp::socket>(io_context_);

    //异步连接
    acceptor_.async_accept(*socket , [this,socket](const boost::system::error_code& error){
        if(!error){
            std::cout<<"Client connected on thread:"<<std::this_thread::get_id()<<std::endl;
           
            std::thread([this,socket](){
                boost::system::error_code error;  // 这里根据实际情况设置error
                this->handle_accept(socket,error);}).detach();//启动新线程处理客户端
        }else{

            LOG_ERROR<<"异步连接出错："<<error.message();
        }
        start_accept();
           
    });

}

void ChatServer::handle_accept(std::shared_ptr<tcp::socket> socket,const boost::system::error_code& error){
    if(!error){
        LOG_INFO<<"有新的客户端连接："<<socket->remote_endpoint();

        //将socket存入客户端列表
        //加锁保护
        {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            clients.push_back(socket);
        }

        //开始读取消息
        start_read(socket);

       
    }
    else{
        LOG_ERROR<<"连接错误！！！错误信息："<<error.message();
    }


    }

void ChatServer::start_read(std::shared_ptr<tcp::socket> socket){
    auto buffer=std::make_shared<std::vector<char> >(1024);

    //异步读取数据
    socket->async_read_some(boost::asio::buffer(*buffer),[this,socket,buffer](const boost::system::error_code& error,size_t bytes){
        handle_read(socket,buffer,error,bytes);

    });

}

void ChatServer::handle_read(std::shared_ptr<tcp::socket> socket,std::shared_ptr<std::vector<char> > buffer,const boost::system::error_code& error,size_t bytes){
    if(!error && bytes>0){
        std::string message(buffer->begin(),buffer->begin()+bytes);
        LOG_INFO<<"收到消息："<<message;

        // 获取数据库连接
        auto& pool = ConnectionPool<MySQLAdapter>::getInstance();
        auto conn = pool.getConnection();

        try{

            conn->beginTransaction();
            
            // 解析消息格式：类型|内容 (示例："MSG|receiver_id|content")
            std::istringstream iss(message);
            std::string msgType;
            getline(iss, msgType, '|');
            
            if(msgType == "MSG") {
                int sender_id, receiver_id;
                std::string content;
                iss >> sender_id >> receiver_id;
                getline(iss, content);
                
                // 存储消息
                std::string query = "INSERT INTO messages (sender_id, receiver_id, content) VALUES (" + 
                                    std::to_string(sender_id) + "," + std::to_string(receiver_id) + ",'" + content + "')";
                if(!conn->execute(query)) throw std::runtime_error("消息存储失败");
                
                // 更新用户状态
                std::string updateStatus = "UPDATE user_status SET last_active=NOW() WHERE user_id=" + 
                                          std::to_string(sender_id);
                if(!conn->execute(updateStatus)) throw std::runtime_error("状态更新失败");
            }
            
            conn->commit();
            boardcast(message);  // 广播成功消息

        }
        catch(const std::exception& e){
            conn->rollback();
            LOG_ERROR << "事务失败: " << e.what();
        }


        start_read(socket);

    }else{
        LOG_ERROR<<"读取错误！客户端断开："<<socket->remote_endpoint();

        //从客户端列表移除，加锁保护clients
        {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            auto it=std::find(clients.begin(),clients.end(),socket);
           if(it!=clients.end()){
              clients.erase(it);
           }

        }
    }

}

void ChatServer::boardcast(const std::string& message){
    //加锁保护clients
    std::lock_guard<std::mutex> lock(clients_mutex_);
    for(auto &client : clients){
        boost::asio::async_write(*client,boost::asio::buffer(message+"\n"),[](const boost::system::error_code&,size_t){});

    }
}



