#include <stdexcept>

#include "MySQLAdapter.h"
#include "../log/logger.h"

//构造函数
MySQLAdapter::MySQLAdapter():m_conn(nullptr){}

//析构函数
MySQLAdapter::~MySQLAdapter(){
    if(m_conn){
        mysql_close(m_conn);
    }
}

bool MySQLAdapter::connect(const std::string &connStr){
    

    // m_conn=mysql_init(nullptr);
    // if(!m_conn){
    //     LOG_ERROR<<"MySQL 初始化失败!!!";
    //     return false;
    // }
    // std::string host="localhost";
    // std::string user="root";
    // std::string password="304474";
    // std::string database="db";

    // if(!mysql_real_connect(m_conn,host.c_str(),user.c_str(),password.c_str(),database.c_str(),3306,nullptr,0)){
    //     LOG_ERROR<<"MySQL 连接失败!!!"<<mysql_error(m_conn);
    //     return false;

    // }
    
    // LOG_INFO<<"MySQL 连接成功了!!!";
    // return true;
    std::regex pattern(R"(mysql://(\w+):(\w+)@([\w.]+):(\d+)/(\w+))");
    std::smatch matches;

    if(!regex_match(connStr, matches, pattern)) {
        LOG_ERROR << "Invalid connection string format";
        return false;
    }

    std::string user = matches[1];
    std::string password = matches[2];
    std::string host = matches[3];
    int port = stoi(matches[4]);
    std::string database = matches[5];

    m_conn = mysql_init(nullptr);
    if(!mysql_real_connect(m_conn, host.c_str(), user.c_str(), password.c_str(),
                         database.c_str(), port, nullptr, CLIENT_MULTI_STATEMENTS)) {
        LOG_ERROR << "MySQL连接失败: " << mysql_error(m_conn);
        return false;
    }
    

    // 启用自动重连
    my_bool reconnect = 1;
    mysql_options(m_conn, MYSQL_OPT_RECONNECT, &reconnect);
    
    LOG_INFO << "成功连接到MySQL节点: " << host << ":" << port;
    return true;

}

bool MySQLAdapter::execute(const std::string & query){
    if(!m_conn){
        LOG_ERROR<<"Database 没有成功连接!!!";
        return false;
    }

    if(mysql_query(m_conn,query.c_str())){
        LOG_ERROR<<"Query 失败！！！"<<mysql_error(m_conn);
        return false;
    }
    LOG_INFO<<"Query 处理成功!!!"<<query;
    return true;
}

bool MySQLAdapter::beginTransaction(){
    return execute("START TRANSACTION");
}

bool MySQLAdapter::commit(){
    return execute("COMMIT");
}

bool MySQLAdapter::rollback(){
    return execute("ROLLBACK");
}





