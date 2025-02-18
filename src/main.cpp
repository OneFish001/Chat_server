#include "ChatServer.h"

int main(){
    try{
        boost::asio::io_context io_context;
        ChatServer server(io_context,8888,4);//设置四个线程
        // io_context.run();//启动事件循环
        // std::cout<<"Success!"<<std::endl;

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
        std::cout<<"发生异常啦："<<e.what()<<"\n";

    }
    return 0;
}