main: src/* src/windows/* include/*
	g++ -o main \
		src/main.cpp \
		src/solver.cpp \
		src/windows/sudokuWin.cpp \
		src/windows/buttonsWin.cpp \
		-Iinclude -lncurses

run: main
	./main