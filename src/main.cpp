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

SudokuWin sudokuInputWin;
ButtonsWin buttonsWin;
array<InteractiveWin*, 2> windows { &sudokuInputWin, &buttonsWin };
int focusedWin { 0 };
SudokuWin sudokuOutputWin;
InfoWin infoWin;
SudokuSolver solver;
bool shouldExit = false;

int run();
void applicationLoop();
void onSolveButtonClick();
void onClearButtonClick();
void onExitButtonClick();
void onToggleAnimationButtonClick();
void solverOnAfterCollapse(const SolveInfo &data);

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
    sudokuInputWin.init(0, 0, blondePlatine, "Input");
    sudokuInputWin.focus();

    sudokuOutputWin.init(0, 25, Sudoku{}, "Output");
    sudokuOutputWin.blur();

    buttonsWin.init(13, 0);
    buttonsWin.setButtonCallback("solve", &onSolveButtonClick);
    buttonsWin.setButtonCallback("clear", &onClearButtonClick);
    buttonsWin.setButtonCallback("exit", &onExitButtonClick);
    buttonsWin.setButtonCallback("toggle_animation", &onToggleAnimationButtonClick);

    infoWin.init(0, 50);

    // Initialize solver callbacks
    solver.setCallback("after_collapse", &solverOnAfterCollapse);

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

        move(16, 0);
        clrtoeol();
        printw("You pressed '%c' (code %d)", ch, ch);
        refresh();
    }
}

void onSolveButtonClick() {
    infoWin.clear();

    SolveInfo result { solver.solve(sudokuInputWin.getDisplayedSudoku()) };
    if (result.isSolved) {
        sudokuOutputWin.setDisplayedSudoku(result.sudoku);
        infoWin.displayStats(result);
    } else {
        sleep(20);
        infoWin.sayImpossibleSudoku(result);
    }
}

void solverOnAfterCollapse(const SolveInfo &data) {
    sudokuOutputWin.setDisplayedSudoku(data.sudoku);
    infoWin.displayIntermediate(data);
    sleep(250);
}

void onClearButtonClick() {
    sudokuInputWin.setDisplayedSudoku(Sudoku{});
}

void onToggleAnimationButtonClick() {
    solver.runCallbacks = !solver.runCallbacks;
    buttonsWin.setButtonText(
        "toggle_animation", 
        solver.runCallbacks ? "Disable animation" : "Enable animation ");
}

void onExitButtonClick() {
    shouldExit = true;
}