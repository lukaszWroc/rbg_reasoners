all:
	g++ -Wall -Wextra -Wpedantic -std=c++17 -Ofast -march=native -flto -fconstexpr-ops-limit=935544320 -s -DNDEBUG -DRBG_RANDOM_GENERATOR=0 test/reasoner.cpp test/benchmark.cpp -o benchmark
	g++ -Wall -Wextra -Wpedantic -std=c++17 -Ofast -march=native -flto -fconstexpr-ops-limit=935544320 -DRBG_RANDOM_GENERATOR=0 test/reasoner.cpp mcts.cpp  -o mcts

clean:
	rm  benchmark mcts
