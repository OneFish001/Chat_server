#pragma once
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

#include "DatabaseAdapter.h"
#include "MySQLAdapter.h"

template<typename T>
class ConnectionPool{


public:

    static ConnectionPool& getInstance(){
        static ConnectionPool instance;
        return instance;
    }

    // 删除拷贝构造函数和赋值运算符
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;


    std::shared_ptr<DatabaseAdapter> getConnection(){
        std::unique_lock<std::mutex> lock(mtx_);
        if(pool_.empty()) {
            if(current_size_ < max_size_) {
                // auto conn = std::make_shared<T>();//从集群中选择节点
                // conn->connect(confstr);//
                // current_size_++;
                auto conn=createNewConnection();
                if(conn) return conn;
            }
            condition_.wait(lock, [this]{ return !pool_.empty(); });
        }
        auto conn = pool_.front();
        pool_.pop();
        return conn;
    }

    //返回连接
    void returnConnection(std::shared_ptr<DatabaseAdapter> conn){
        std::unique_lock<std::mutex> lock(mtx_);
        pool_.push(conn);
        condition_.notify_one();
    }

private:
    
    ConnectionPool() : current_size_(0), max_size_(10) {
    // 初始化连接池
    for (int i = 0; i < max_size_; ++i) {

        auto conn=createNewConnection();
        if(conn) pool_.push(conn);
}

}

std::shared_ptr<DatabaseAdapter> createNewConnection() {
    std::string connStr = nodes_[current_node_];
    current_node_ = (current_node_ + 1) % nodes_.size();
    
    auto conn = std::make_shared<T>();
    if(conn->connect(connStr)) {
        current_size_++;
        return conn;
    }
    return nullptr; // 节点故障处理
}

    std::mutex mtx_;
    std::condition_variable condition_;
    int max_size_;
    size_t current_node_=0;
    std::queue<std::shared_ptr<DatabaseAdapter>> pool_;
    std::vector<std::string>  nodes_={ "mysql://root:304474@node1:3306/db",
        "mysql://root:304474@node2:3306/db"};
   

    int current_size_;


};

