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
                auto conn = std::make_shared<T>();
                conn->connect(confstr);
                current_size_++;
                return conn;
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
                            auto conn = std::make_shared<T>();
                            conn->connect(confstr);
                            pool_.push(conn);
                        }
                    }

                    std::mutex mtx_;
                    std::condition_variable condition_;
                    int max_size_;
                    int current_size_;
                    std::queue<std::shared_ptr<DatabaseAdapter>> pool_;
                    const std::string confstr = "mysql://user:password@localhost:3306/db";


};

