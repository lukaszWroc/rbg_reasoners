all:
	g++ -Wall -Wextra -Wpedantic -std=c++17 -Ofast -march=native -flto -s -DNDEBUG -DRBG_RANDOM_GENERATOR=0 test/reasoner.cpp test/benchmark.cpp -o benchmark

clean:
	rm test/reasoner.cpp test/reasoner.hpp benchmark
	rm -rf logs