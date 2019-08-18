libs=$(pkg-config --cflags --libs libmongocxx)

all:test

test:
	clang++ --std=c++11 test.cpp -o test -I/usr/local/include/mongocxx/v_noabi \
    -I/usr/local/include/bsoncxx/v_noabi -L/usr/local/lib -lmongocxx -lbsoncxx

.PHONY:clean
clean:
	rm -rf test
