cxxflag=-I /usr/local/include/libmongoc-1.0/ -I /usr/local/include/libbson-1.0/ \
		-lmongoc-1.0 -lmongoc-1.0 -lbson-1.0

libs=$(pkg-config --libs --cflags libmongoc-1.0)

all:test

test:
	g++ BsonTest.cpp ${cxxflag} -o bson.out
	g++ MongoTest.cpp ${cxxflag} -o mongo.out
	g++ InsertTest.cpp ${cxxflag} -o insert.out
	g++ FindTest.cpp ${cxxflag} -o find.out
	g++ Find-Specific.cpp ${cxxflag} -o find-specific.out
	g++ UpdateTest.cpp ${cxxflag} -o update.out
	g++ DeleteTest.cpp ${cxxflag} -o delete.out
	g++ CountTest.cpp ${cxxflag} -o count.out
	g++ ExecuteTest.cpp ${cxxflag} -o execute.out

.PHONY:clean
clean:
	rm -rf *.out