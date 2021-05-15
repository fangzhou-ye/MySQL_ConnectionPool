#include "ConnnectionPool.hpp"

ConnectionPool::ConnectionPool() : _connCounter(0)
{
    if (!loadConf())
    {
        return;
    }
    // 创建初始数量的连接
    for (int i = 0; i < _initSize; ++i)
    {
        Connection *p = new Connection();
        p->connect(_ip, _username, _password, _dbname);
        p->refreshAliveTime(); // 刷新开始空闲的起始时间
        _connQueue.push(p);
        _connCounter++;
    }
    // 启动一个新线程用于新连接生产者
    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();
    // 启动一个新的定时线程，扫描多余的空闲线程（队列内）超过maxIdleTime回收连接
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}

void ConnectionPool::produceConnectionTask()
{
    for (;;)
    {
        unique_lock<mutex> lock(_queueMutex);
        while (!_connQueue.empty())
        {
            cv.wait(lock); // 队列不空，生产线成等待
        }
        // 连接数量未到达上限，创造新连接
        if (_connCounter < _maxSize)
        {
            Connection *p = new Connection();
            p->connect(_ip, _username, _password, _dbname);
            p->refreshAliveTime(); // 刷新开始空闲的起始时间
            _connQueue.push(p);
            _connCounter++;
        }
        cv.notify_all(); // 通知所有消费者线程可以获取新连接
    }
}

void ConnectionPool::scannerConnectionTask()
{
    for (;;)
    {
        // 模拟定时效果
        this_thread::sleep_for(chrono::seconds(_maxIdleTime));
        // 扫描队列（空闲线程）释放多余的连接 (>initSize)
        unique_lock<mutex> lock(_queueMutex);
        while (_connCounter > _initSize)
        {
            Connection *p = _connQueue.front();
            if (p->getAliveTime() >= (_maxIdleTime * 1000))
            {
                _connQueue.pop();
                _connCounter--;
                delete p; // 释放连接调用Connection析构函数
            }
            else
            {
                break; // 队头未超过
            }
        }
    }
}

// 线程安全的懒汉单例函数接口
ConnectionPool *ConnectionPool::getConnectionPool()
{
    static ConnectionPool pool; //静态对象初始化lock unlock
    return &pool;
}

shared_ptr<Connection> ConnectionPool::getConnection()
{
    unique_lock<mutex> lock(_queueMutex);
    while (_connQueue.empty())
    {
        // 因超时被唤醒
        if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeOut)))
        {
            if (_connQueue.empty())
            {
                LOG("fail to acquire connection TIMEOUT !!!");
                return nullptr;
            }
        }
    }
    // 智能指针析构时会把Connection资源delete，调用Connection的析构函数连接被关闭
    // 需要自定义释放资源的方式，把Connection归还到队列中
    shared_ptr<Connection> sp(_connQueue.front(),
                              // 自定义的智能指针析构访达，归还连接
                              [&](Connection *pconn) {
                                  unique_lock<mutex> lock(_queueMutex); // 服务器线程中考虑线程安全
                                  pconn->refreshAliveTime();            // 刷新开始空闲的起始时间
                                  _connQueue.push(pconn);
                              });
    _connQueue.pop();
    cv.notify_all(); // 通知生产者线程队列为空则生产新连接
    return sp;
}

bool ConnectionPool::loadConf()
{
    FILE *pf = fopen("/home/yef/connPool/src/mysql.cnf", "r");
    if (pf == nullptr)
    {
        LOG("mysql.txt not exists");
        return false;
    }
    while (!feof(pf))
    {
        char line[1024] = {0};
        fgets(line, 1024, pf);
        string str = line;
        int idx = str.find('=', 0);
        if (idx == -1)
        {
            continue;
        }
        //xxx=yyy\n
        int endIdx = str.find('\n', idx);
        string key = str.substr(0, idx);
        string value = str.substr(idx + 1, endIdx - idx - 1);
        if (key == "ip")
        {
            _ip = value;
        }
        else if (key == "port")
        {
            _port = stoi(value);
        }
        else if (key == "username")
        {
            _username = value;
        }
        else if (key == "password")
        {
            _password = value;
        }
        else if (key == "dbname")
        {
            _dbname = value;
        }
        else if (key == "initSize")
        {
            _initSize = stoi(value);
        }
        else if (key == "maxSize")
        {
            _maxSize = stoi(value);
        }
        else if (key == "maxIdleTime")
        {
            _maxIdleTime = stoi(value);
        }
        else if (key == "connectionTimeout")
        {
            _connectionTimeOut = stoi(value);
        }
    }
    return true;
}