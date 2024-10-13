#include "sudoku.hpp"
#include "solver.hpp"
#include "grids.hpp"
#include "windows/sudokuWin.hpp"
#include "windows/buttonsWin.hpp"

#include <iostream>
#include <ncurses.h>

using namespace std;

SudokuWin sudokuWin;
ButtonsWin buttonsWin;
int focusedWin { 0 };
SudokuSolver solver;

int run();
void applicationLoop();
void onSolveButtonClick();
void onClearButtonClick();

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

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    standend();
    
    refresh(); // Refresh stdscr (seems necessary for proper initialization of windows)

    // Initialisation of gui
    sudokuWin.init(volodia);
    sudokuWin.focus();

    buttonsWin.init();
    buttonsWin.setButtonCallback("solve", &onSolveButtonClick);
    buttonsWin.setButtonCallback("clear", &onClearButtonClick);

    applicationLoop();

    // Ending ncurses
    endwin();

    return 0;
}

void applicationLoop() {
    int ch;

    while ((ch = getch()) != 4) { // 4 is CTRL_D
        if (ch == '\t') {
            if (focusedWin == 0) {
                sudokuWin.blur();
                buttonsWin.focus();
                focusedWin = 1;
            } else {
                sudokuWin.focus();
                buttonsWin.blur();
                focusedWin = 0;
            }
        } else {
            if (focusedWin == 0) {
                sudokuWin.onKey(ch);
            } else {
                buttonsWin.onKey(ch);
            }
        }

        move(13, 0);
        clrtoeol();
        printw("You pressed '%c' (code %d)", ch, ch);
        refresh();
    }
}

void onSolveButtonClick() {
    SudokuResult result { solver.solve(sudokuWin.getDisplayedSudoku()) };
    sudokuWin.setDisplayedSudoku(result.sudoku);
}

void onClearButtonClick() {
    sudokuWin.setDisplayedSudoku(Sudoku{});
}