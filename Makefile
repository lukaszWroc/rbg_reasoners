CFLAGS = -Wall -Wextra -Wpedantic -std=c++17 -Ofast -march=native -flto -fconstexpr-ops-limit=935544320 -s -DNDEBUG -DRBG_RANDOM_GENERATOR=0

all: benchmark test

benchmark-monotonic:
	g++ ${CFLAGS} -DMONOTONIC test/reasoner.cpp test/benchmark.cpp -o benchmark

benchmark:
	g++ ${CFLAGS} test/reasoner.cpp test/benchmark.cpp -o benchmark

perft:
	g++ ${CFLAGS} test/reasoner.cpp test/mcts.cpp -o perft

clean-benchmark:
	rm benchmark

clean-perft:
	rm perft

clean: clean-benchmark clean-perft
