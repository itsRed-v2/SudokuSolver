main: src/main.cpp src/solver.cpp include/solver.hpp
	g++ -o main src/main.cpp src/solver.cpp -Iinclude

run: main
	./main