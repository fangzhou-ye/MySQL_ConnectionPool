#ifndef CONNECTION_H
#define CONNECTION_H

#include <mysql/mysql.h>
#include <string>
#include <ctime>

using namespace std;

class Connection
{
public:
    // 初始化数据库连接
    Connection();
    // 书房数据库连接资源
    ~Connection();
    // 连接数据库
    bool connect(string server, string user, string password, string dbname);
    // 更新操作
    bool update(string sql);
    // 查询操作
    MYSQL_RES *query(string sql);
    // 获取连接
    MYSQL *getConnection();
    // 刷新连接的空闲存活时间点
    void refreshAliveTime() { _aliveTime = clock(); }
    // 返回存活时间
    clock_t getAliveTime() { return clock() - _aliveTime; }

private:
    MYSQL *_conn;
    clock_t _aliveTime; // 记录进入空闲状态的存活起始时间点
};

#endif