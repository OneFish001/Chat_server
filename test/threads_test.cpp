#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <vector>

void task(boost::asio::io_context& io_context) {
    std::cout << "Task executed by thread: " << std::this_thread::get_id() << std::endl;
}

int main() {
    boost::asio::io_context io_context;

    // 提交一些任务
    for (int i = 0; i < 8; ++i) {
        io_context.post([&io_context]() { task(io_context); });
    }
