#include "Pool.h"

// 这样可以保证 当conn生命周期大于pool时的情况
void delter(ConnBase *conn){
    auto ptr = conn->getPool();
    if(ptr){
        ptr->poolReuse(conn);
    }else{
        delete conn;
    }
}