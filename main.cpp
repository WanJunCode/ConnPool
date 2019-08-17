#include "Pool.h"
#include "MyConn.h"
#include <stdio.h>
#include <assert.h>

int main(){
    std::shared_ptr<ConnBase> g_conn;
    auto pool = std::make_shared<ConnPool>();
    ConnFactory *factory = new myFactory();
    {
        // ConnFactory *factory = new ConnFactory();
        pool->init(factory,5);
        assert(pool->getSize()==5);

        auto conn = pool->grab();
        assert(pool->getSize()==4);

        conn->exec();

        auto conn1 = pool->grab();
        assert(pool->getSize()==3);
        auto conn2 = pool->grab();
        assert(pool->getSize()==2);
        auto conn3 = pool->grab();
        assert(pool->getSize()==1);
        auto conn4 = pool->grab();
        assert(pool->getSize()==0);

        auto conn5 = pool->grab();
        auto conn6 = pool->grab();
        auto conn7 = pool->grab();
        auto conn8 = pool->grab();
        auto conn9 = pool->grab();
    }
    assert(pool->getSize()==6);

    return 0;
}