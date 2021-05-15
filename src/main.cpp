#include <iostream>
#include "Connection.hpp"
#include "ConnnectionPool.hpp"

int main(){
    // Connection conn;
    // conn.connect("127.0.0.1", "root", "123", "test");
    // ConnectionPool *cp = ConnectionPool::getConnectionPool();
    // // no pool 1000
    int nb = 10000;
    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO user(name, age, sex) VALUES('%s', '%d', '%s');", "zhang san", 20, "male");
    auto start = chrono::steady_clock::now();

    // for(int i=0; i<nb; ++i)
    // {
    //     Connection conn;
    //     conn.connect("127.0.0.1", "root", "123", "test");
    //     conn.update(sql);
    // }
    ConnectionPool *cp = ConnectionPool::getConnectionPool();
    for(int i=0; i<nb; ++i)
    {
        shared_ptr<Connection> sp = cp->getConnection();
        sp->update(sql);
    }
    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in milliseconds: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;
    return 0;
}


// for(int i=0; i<nb; ++i)
    // {
    //     Connection conn;
    //     conn.connect("127.0.0.1", "root", "123", "test");
    //     conn.update(sql);
    // }
    // for(int i=0; i<nb; ++i)
    // {
    //     shared_ptr<Connection> sp = cp->getConnection();
    //     sp->update(sql);
    // }


// thread t1([&](){
    //     // ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     char sql[1024] = {0};
    //     sprintf(sql, "INSERT INTO user(name, age, sex) VALUES('%s', '%d', '%s');", "zhang san", 20, "male");
    //     for(int i=0; i<250; ++i)
    //     {
    //         // shared_ptr<Connection> sp = cp->getConnection();
    //         // sp->update(sql);
    //         // Connection conn;
    //         // conn.connect("127.0.0.1", "root", "123", "test");
    //         conn.update(sql);
    //     }
    // });

    // thread t2([&](){
    //     // ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     char sql[1024] = {0};
    //     sprintf(sql, "INSERT INTO user(name, age, sex) VALUES('%s', '%d', '%s');", "zhang san", 20, "male");
    //     for(int i=0; i<250; ++i)
    //     {
    //         // shared_ptr<Connection> sp = cp->getConnection();
    //         // sp->update(sql);
    //         // Connection conn;
    //         // conn.connect("127.0.0.1", "root", "123", "test");
    //         conn.update(sql);
    //     }
    // });

    // thread t3([&](){
    //     // ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     char sql[1024] = {0};
    //     sprintf(sql, "INSERT INTO user(name, age, sex) VALUES('%s', '%d', '%s');", "zhang san", 20, "male");
    //     for(int i=0; i<250; ++i)
    //     {
    //         // shared_ptr<Connection> sp = cp->getConnection();
    //         // sp->update(sql);
    //         // Connection conn;
    //         // conn.connect("127.0.0.1", "root", "123", "test");
    //         conn.update(sql);
    //     }
    // });

    // thread t4([&](){
    //     // ConnectionPool *cp = ConnectionPool::getConnectionPool();
    //     char sql[1024] = {0};
    //     sprintf(sql, "INSERT INTO user(name, age, sex) VALUES('%s', '%d', '%s');", "zhang san", 20, "male");
    //     for(int i=0; i<250; ++i)
    //     {
    //         // shared_ptr<Connection> sp = cp->getConnection();
    //         // sp->update(sql);
    //         // Connection conn;
    //         // conn.connect("127.0.0.1", "root", "123", "test");
    //         conn.update(sql);
    //     }
    // });

    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();