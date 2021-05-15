#include "Connection.hpp"
#include "public.hpp"

// 数据库配置信息
// static string server = "127.0.0.1";
// static string user = "root";
// static string password = "123";
// static string dbname = "test";

Connection::Connection()
{
    _conn = mysql_init(nullptr);
}

Connection::~Connection()
{
    if (_conn != nullptr)
        mysql_close(_conn);
}

// 连接数据库
bool Connection::connect(string server, string user, string password, string dbname)
{
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
                                  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        // 默认编码是ASCII
        mysql_query(_conn, "set name gbk");
        // LOG("connect mysql success!");
    }
    else
    {
        LOG("connect mysql fail!");
    }
    return p;
}

// 更新操作
bool Connection::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG("update fail: " + sql);
        return false;
    }
    return true;
}

// 查询操作
MYSQL_RES *Connection::query(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG("query fail: " + sql);
        return nullptr;
    }
    return mysql_use_result(_conn);
}

MYSQL *Connection::getConnection()
{
    return _conn;
}