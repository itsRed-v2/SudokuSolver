#include "sudoku.hpp"
#include "solver.hpp"
#include "grids.hpp"
#include "windows/sudokuWin.hpp"
#include "windows/buttonsWin.hpp"
#include "windows/infoWin.hpp"
#include "colors.hpp"

#include <iostream>
#include <array>
#include <ncurses.h>
#include <chrono>
#include <thread>

using namespace std;

#define sleep(ms) this_thread::sleep_for(chrono::milliseconds(ms))

SudokuWin sudokuWin;
ButtonsWin buttonsWin;
array<InteractiveWin*, 2> windows { &sudokuWin, &buttonsWin };
int focusedWin { 0 };
InfoWin infoWin;
SudokuSolver solver;
bool shouldExit = false;

int run();
void applicationLoop();
void onSolveButtonClick();
void onClearButtonClick();
void onExitButtonClick();

int main() {
    try {
        return run();
    } catch(const std::exception& e) {
        endwin();
        cerr << "Something went wrong:" << endl;
        cerr << e.what() << endl;
    }

    return 1;
}

int run() {
    // Initialisation of ncurses
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Make the cursor invisible
    initNcursesColors();
    refresh(); // Refresh stdscr (seems necessary for proper initialization of windows)

    // Initialisation of gui
    sudokuWin.init(easySudoku);
    sudokuWin.focus();

    buttonsWin.init();
    buttonsWin.setButtonCallback("solve", &onSolveButtonClick);
    buttonsWin.setButtonCallback("clear", &onClearButtonClick);
    buttonsWin.setButtonCallback("exit", &onExitButtonClick);

    infoWin.init();

    applicationLoop();

    // Ending ncurses
    endwin();

    return 0;
}

void applicationLoop() {
    int ch;

    while ((ch = getch()) != 4) { // 4 is CTRL_D
        if (ch == '\t') {
            windows[focusedWin]->blur();
            focusedWin = (focusedWin + 1) % windows.size();
            windows[focusedWin]->focus();
        } else {
            windows[focusedWin]->onKey(ch);
        }

        if (shouldExit) break;

        move(13, 0);
        clrtoeol();
        printw("You pressed '%c' (code %d)", ch, ch);
        refresh();
    }
}

void onSolveButtonClick() {
    infoWin.clear();

    SudokuResult result { solver.solve(sudokuWin.getDisplayedSudoku()) };
    if (result.isSolved) {
        sudokuWin.setDisplayedSudoku(result.sudoku);
        infoWin.displayStats(result);
    } else {
        sleep(20);
        infoWin.sayImpossibleSudoku(result);
    }
}

void onClearButtonClick() {
    sudokuWin.setDisplayedSudoku(Sudoku{});
}

void onExitButtonClick() {
    shouldExit = true;
}