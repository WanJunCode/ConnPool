all:pool

pool:
	clang++ main.cpp Pool.cpp -o pool

.PHONY:clean
clean:
	rm -rf pool
