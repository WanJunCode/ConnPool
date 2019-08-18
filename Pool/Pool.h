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
    typedef void (*deleteFunc)(ConnBase *conn);

public:
    ConnPool(deleteFunc func = delter){
        mdelter = func;
        mfactory = nullptr;
    }

    ~ConnPool(){
        Clear();
    }

    // for debug
    size_t getSize(){
        return connList.size();
    }

    void Clear(){
        while (!connList.empty()){
            auto ptr = connList.front();
            connList.pop_front();
            delete ptr;
            ptr = nullptr;
        }
    }

    // 通过初始化不同的 使用创建工厂实现多态
    void init(ConnFactory *factory, size_t num){
        if(!mfactory){
            // 第一次设置 factory
            mfactory = factory;
            mCount = num;
            for(size_t i=0;i<num;i++){
                connList.emplace_back(factory->create(shared_from_this()));
            }
        }else{
            // 重复使用 init 函数
            ::exit(EXIT_FAILURE);
        }
    }

    void setCallBack(deleteFunc func){
        mdelter = func;
    }

    // grab 是一直会得到连接的，除非 factory 创建失败
    ConnPtr grab(){
        ConnBase *conn;
        if(!connList.empty()){
            conn = connList.front();
            connList.pop_front();
        }else{
            conn = mfactory->create(shared_from_this());
        }
        std::shared_ptr<ConnBase> ptr(conn,mdelter);
        return ptr;
    }
    
    // 回收
    void poolReuse(ConnBase *conn){
        if(connList.size()<(mCount * decrRate)){
            connList.emplace_back(conn);
        }else{
            printf("当前已经存放了 [%lu] 个连接\n",connList.size());
            // 当前的存放值不能大于 (1 + decrRate)%
            delete conn;
        }
    }

private:
    ConnFactory *mfactory;
    std::list<ConnBase *> connList;
    deleteFunc mdelter;
    size_t mCount;
    float incrRate;         // 动态变更因子
    float decrRate = 1.2;
};

#endif //CONN_POOL_H