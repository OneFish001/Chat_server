#include "../include/ChatServer.h"
#include "db_adapter/ConnectionPool.h"
#include "log/logger.h"

int main(){
    //初始化日志
    initLogger();

    try{
        boost::asio::io_context io_context;
        ChatServer server(io_context,8888,4);//设置四个线程
        // io_context.run();//启动事件循环
        // std::cout<<"Success!"<<std::endl;

        //数据库连接
        // ConnectionPool pool;

        auto &pool =ConnectionPool<MySQLAdapter>::getInstance();
        auto conn=pool.getConnection();

       
        conn->execute("INSERT INTO logs VAlUES (...)");
        pool.returnConnection(conn);


        //创建线程库
        std::vector<std::thread>  threads;
        for(int i=0;i<4;i++){
            threads.emplace_back([&io_context]{
                io_context.run();

            });
        }

        //等待所有线程结束
        for(auto& t:threads){
            t.join();
        }

    }catch(std::exception &e){
        // std::cout<<"发生异常啦："<<e.what()<<"\n";
        LOG_ERROR<<"服务器异常！！！"<<e.what();

    }
    return 0;
}