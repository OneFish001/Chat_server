#include <stdexcept>

#include "MySQLAdapter.h"
// #include "ConnectionPool.h"
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
    m_conn=mysql_init(nullptr);
    if(!m_conn){
        LOG_ERROR<<"MySQL 初始化失败!!!!!!!!!!!!!!!!!!!!!";
        return false;
    }
    std::string host="localhost";
    std::string user="root";
    std::string password="123456hou";
    std::string database="db";

    if(!mysql_real_connect(m_conn,host.c_str(),user.c_str(),password.c_str(),database.c_str(),3306,nullptr,0)){
        LOG_ERROR<<"MySQL 连接失败！！！！！！！！！！！！"<<mysql_error(m_conn);
        return false;

    }
    
    LOG_INFO<<"MySQL 连接成功了！！！！！！！！！！！";
    return true;
}

bool MySQLAdapter::execute(const std::string & query){
    if(!m_conn){
        LOG_ERROR<<"Database 没有成功连接！！！！！！！！！！！！！！！！！";
        return false;
    }

    if(mysql_query(m_conn,query.c_str())){
        LOG_ERROR<<"Query 失败！！！"<<mysql_error(m_conn);
        return false;
    }
    LOG_INFO<<"Query 处理成功！！！！！！！！！！！！！！"<<query;
    return true;
}

bool MySQLAdapter::beginTransaction(){
    return execute("开始 Transaction!!!!!!!!!!!!!!");
}

bool MySQLAdapter::commit(){
    return execute("COMMIT!!!!!!!!!!!!!!!!!!!!!");
}

bool MySQLAdapter::rollback(){
    return execute("ROLLBACK!!!!!!!!!!!!!");
}





