#include "ChatServer.h"

int main(){
    try{
        boost::asio::io_context io_context;
        ChatServer server(io_context,8888);
        io_context.run();//启动事件循环

    }catch(std::exception &e){
        std::cout<<"发生异常啦："<<e.what()<<"\n";

    }
    return 0;
}