#pragma once
#include <string>

class DatabaseAdapter{
public:
    virtual ~DatabaseAdapter()=default;

    //规定规范
    virtual bool connect(const std::string &connStr)=0;
    virtual bool execute(const std::string &query)=0;

    //
    virtual bool beginTransaction()=0;//开始
    virtual bool commit()=0;//确认动作完成
    virtual bool rollback()=0;//紧急回收

};

