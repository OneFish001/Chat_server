 基于Boost的聊天服务器
 基本架构：
 
                            ┌───────────────────┐
                            │   Boost.Asio      │
                            │  TCP Server       │
                            └─────────┬─────────┘
                                      │
                                      ▼
┌───────────────────┐       ┌───────────────────┐       ┌───────────────────┐
│   Client Threads  │──────▶│  Database Adapter │──────▶│ MySQL Cluster     │
│  (Waiters Team)   │       │  (Supply Chain)   │       │ (Central Kitchen) │
└───────────────────┘       └───────────────────┘       └───────────────────┘
           │                           │
           │                           │
           ▼                           ▼
┌───────────────────┐       ┌───────────────────┐
│  Boost.Log        │       │   Connection Pool │
│  (Surveillance)   │       │   (Warehouse)     │
└───────────────────┘       └───────────────────┘

完整系统交互流程图：

客户端请求
  │
  ▼
线程池处理 → 从连接池获取数据库连接（加锁）
  │                  │
  │                  ▼
  │          执行SQL事务（ACID保障）
  │                  │
  ▼                  ▼
广播消息（加锁）     记录日志（异步安全）
  │                  │
  ▼                  ▼
客户端响应         日志文件滚动存储