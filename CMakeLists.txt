cmake_minimum_required(VERSION 3.10)
project(ChatServer)


# 设置源代码目录和二进制目录
set(INCLUDE_DIR ../include)
set(SRC_DIR ../src)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# 设置 Boost 路径
set(BOOST_ROOT "/usr/local/boost_1_82_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/boost")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/libs")

# 设置 Boost 的头文件路径
include_directories("/usr/local/boost_1_82_0")
# 设置 Boost 的头文件路径
include_directories("/usr/local/boost_1_82_0/libs")
# 设置 Boost 的库文件路径
link_directories("/usr/local/lib")

# 添加库目录
include_directories(/usr/local/mysql-8.0.30-macos12-x86_64/include)
link_directories(/usr/local/mysql-8.0.30-macos12-x86_64/lib)

include_directories(/usr/local//usr/local/boost_1_82_0/boost/include)
link_directories(/usr/local//usr/local/boost_1_82_0/boost/lib)

include_directories(${INCLUDE_DIR})
# 包含 Boost 头文件目录
include_directories(${Boost_INCLUDE_DIRS})





# 添加可执行文件
add_executable(ChatServer ${SRC_DIR}/main.cpp ${SRC_DIR}/ChatServer.cpp)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
# 链接库
# target_link_libraries(ChatServer Boost::Boost)

