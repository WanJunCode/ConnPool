#include "Pool.h"
#include "MyConn.h"
#include <stdio.h>


int main(){
    std::shared_ptr<ConnBase> g_conn;
    {
        auto pool = std::make_shared<Pool>();
        ConnFactory *factory = new ConnFactory();
        // ConnFactory *factory = new myFactory();
        pool->init(factory);
        auto conn = pool->grab();
        conn->exec();
        // conn->exec();
        // g_conn = conn;
    }

    return 0;
}