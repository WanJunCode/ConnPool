#ifndef CONN_POOL_H
#define CONN_POOL_H

#include <memory>
#include <list>
#include <functional>
#include <stdio.h>

using namespace std::placeholders;

class Pool;

// 连接器基类
class ConnBase{
public:
    ConnBase(std::shared_ptr<Pool> p){
        pool = p;
    }

    virtual ~ConnBase(){
    }

    virtual void exec(){
        printf("base conn exec\n");
    }

    std::shared_ptr<Pool> getPool(){
        return pool.lock();
    }

protected:
    std::weak_ptr<Pool> pool;
};

// 创建工厂
class ConnFactory{
public:
    virtual ConnBase *create(std::shared_ptr<Pool> pool){
        return new ConnBase(pool);
    }
};

// 自定义删除器
void delter(ConnBase *conn);

class Pool: public std::enable_shared_from_this<Pool>{
    typedef std::shared_ptr<ConnBase> ConnPtr;
public:
    Pool(){

    }

    ~Pool(){
        while (!connList.empty()){
            auto ptr = connList.front();
            connList.pop_front();
            delete ptr;
            ptr = nullptr;
        }
    }

    // 通过初始化不同的 创建工厂实现多态
    void init(ConnFactory *factory){
        for(int i=0;i<20;i++){
            connList.emplace_back(factory->create(shared_from_this()));
        }
    }

    ConnPtr grab(){
        if(!connList.empty()){
            auto conn = connList.front();
            connList.pop_front();
            std::shared_ptr<ConnBase> ptr(conn,delter);
            return ptr;
        }
        return nullptr;
    }
    
    // 回收
    void poolReuse(ConnBase *conn){
        connList.emplace_back(conn);
    }

private:
    std::list<ConnBase *> connList;
};

#endif //CONN_POOL_H