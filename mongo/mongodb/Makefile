cxxflag=-I /usr/local/include/libmongoc-1.0/ -I /usr/local/include/libbson-1.0/ \
		-lmongoc-1.0 -lmongoc-1.0 -lbson-1.0

libs=$(pkg-config --libs --cflags libmongoc-1.0)

all:test

test:
	clang++ BsonTest.cpp ${cxxflag} -o bson.out
	clang++ MongoTest.cpp ${cxxflag} -o mongo.out
	clang++ InsertTest.cpp ${cxxflag} -o insert.out
	clang++ FindTest.cpp ${cxxflag} -o find.out
	clang++ Find-Specific.cpp ${cxxflag} -o find-specific.out
	clang++ UpdateTest.cpp ${cxxflag} -o update.out
	clang++ DeleteTest.cpp ${cxxflag} -o delete.out
	clang++ CountTest.cpp ${cxxflag} -o count.out
	clang++ ExecuteTest.cpp ${cxxflag} -o execute.out
	clang++ ThreadTest.cpp ${cxxflag} -o thread.out -lpthread

.PHONY:clean
clean:
	rm -rf *.out