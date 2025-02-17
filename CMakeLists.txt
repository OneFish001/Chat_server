# 设置包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)


# 你可能还有类似如下的代码来指定源文件
add_executable(ChatServer
    src/ChatServer.cpp
)