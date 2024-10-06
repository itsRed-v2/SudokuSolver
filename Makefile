main: main.cpp solver.cpp solver.hpp
	g++ -o main main.cpp solver.cpp

run: main
	./main