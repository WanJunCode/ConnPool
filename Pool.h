#ifndef CONN_POOL_H
#define CONN_POOL_H

#include <memory>
#include <list>
#include <functional>
#include <stdio.h>

class ConnPool;

// 连接器基类
class ConnBase{
public:
    ConnBase(std::shared_ptr<ConnPool> p){
        pool = p;
    }

    virtual ~ConnBase(){
    }

    virtual void exec(){
        printf("base conn exec\n");
    }

    std::shared_ptr<ConnPool> getPool(){
        return pool.lock();
    }

protected:
    std::weak_ptr<ConnPool> pool;
};

// 创建工厂
class ConnFactory{
public:
    virtual ConnBase *create(std::shared_ptr<ConnPool> pool){
        return new ConnBase(pool);
    }
};

// 自定义删除器
void delter(ConnBase *conn);

class ConnPool: public std::enable_shared_from_this<ConnPool>{
    typedef std::shared_ptr<ConnBase> ConnPtr;
public:
    ConnPool(){

    }

    ~ConnPool(){
        while (!connList.empty()){
            auto ptr = connList.front();
            connList.pop_front();
            delete ptr;
            ptr = nullptr;
        }
    }

    // 通过初始化不同的 创建工厂实现多态
    void init(ConnFactory *factory){
        mfactory = factory;
        for(int i=0;i<20;i++){
            connList.emplace_back(factory->create(shared_from_this()));
        }
    }

    ConnPtr grab(){
        ConnBase *conn;
        if(!connList.empty()){
            conn = connList.front();
            connList.pop_front();
        }else{
            conn = mfactory->create(nullptr);
        }
        std::shared_ptr<ConnBase> ptr(conn,delter);
        return ptr;
    }
    
    // 回收
    void poolReuse(ConnBase *conn){
        connList.emplace_back(conn);
    }

private:
    ConnFactory *mfactory;
    std::list<ConnBase *> connList;
    float incrRate;         // 动态变更因子
    float decrRate;
};

#endif //CONN_POOL_H