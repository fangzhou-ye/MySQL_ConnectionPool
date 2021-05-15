#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <memory>
#include <functional>
#include "Connection.hpp"
#include "public.hpp"
using namespace std;

class ConnectionPool
{
public:
    // 获取连接池单例实例
    static ConnectionPool* getConnectionPool();
    // 获取一个可用的控线连接 (自动析构)
    shared_ptr<Connection> getConnection();
private:
    //单例 构造函数私有化
    ConnectionPool();
    // 配置文件加载
    bool loadConf();
    // 运行在独立线程负责生产新连接
    void produceConnectionTask();
    // 扫描多余的空闲线程（队列内）超过maxIdleTime回收连接
    void scannerConnectionTask();
private:
    string _ip;
    unsigned short _port;
    string _username;
    string _password;
    string _dbname;
    int _initSize; // 初始连接数
    int _maxSize; // 最大连接数
    int _maxIdleTime; // 最大空闲时间
    int _connectionTimeOut; // 获取连接的超时时间

    queue<Connection*> _connQueue; //存储MySQL连接的队列(MT safe)
    mutex _queueMutex; //保证连接队列的线程安全
    atomic<int> _connCounter; //记录已创建的连接数量
    condition_variable cv; // 条件变量用于连接生产线程和连接消费线程的通信
};

#endif