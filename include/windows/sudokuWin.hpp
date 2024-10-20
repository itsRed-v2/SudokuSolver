#ifndef SUDOKU_WIN_HPP
#define SUDOKU_WIN_HPP

#include <ncurses.h>
#include <string>

#include "windows/interactiveWin.hpp"

#include "sudoku.hpp"

using namespace std;

class SudokuWin: public InteractiveWin {
private:
    WINDOW *m_win;
    int m_curCell { 0 };
    bool m_showCurCell { true };
    string m_title;
    Sudoku displayedSudoku;

private:
    void onArrowPress(int code);
    void updateHighlightedCell();
    void drawBorders();
    void drawSudoku();

    void selectNextCell();
    void selectPrevCell();

public:
    void init(int y, int x, const Sudoku &sudoku, string title);

    void onKey(int code); 
    void focus();
    void blur();

    Sudoku getDisplayedSudoku();
    void setDisplayedSudoku(Sudoku sudoku);
};

#endif