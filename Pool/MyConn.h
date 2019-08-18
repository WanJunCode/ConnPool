#ifndef MY_CONN_H
#define MY_CONN_H

#include "Pool.h"

class myconn: public ConnBase{
public:
    myconn(std::shared_ptr<ConnPool> pool)
        :ConnBase(pool){
    }

    ~myconn(){
        printf("my conn dtor\n");
    }

    virtual void exec() override{
        printf("my conn exec\n");
    }
};

class myFactory: public ConnFactory{
public:
    virtual myconn *create(std::shared_ptr<ConnPool> pool) override{
        return new myconn(pool);
    }
};

#endif //MY_CONN_H