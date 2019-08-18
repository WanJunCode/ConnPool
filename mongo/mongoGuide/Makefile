cxxflag=-I /usr/local/include/libmongoc-1.0/ -I /usr/local/include/libbson-1.0/ \
		-lmongoc-1.0 -lmongoc-1.0 -lbson-1.0
all:pool

pool:
	clang++ ReplicaSet.cpp -o replica.out ${cxxflag}

.PHONY:clean
clean:
	rm -rf *.out
