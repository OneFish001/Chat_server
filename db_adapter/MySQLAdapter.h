#pragma once
#include "DatabaseAdapter.h"
#include <mysql.h> 


class MySQLAdapter : public DatabaseAdapter{

    // MYSQL* conn_ =nullptr;

public:
    MySQLAdapter();
    ~MySQLAdapter();

    bool connect(const std::string& connStr) override;
    bool execute(const std::string& query) override;
    bool beginTransaction() override;
    bool commit() override;
    bool rollback() override;

    
private:
    MYSQL* m_conn;

};
