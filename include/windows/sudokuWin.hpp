#ifndef SUDOKU_WIN_HPP
#define SUDOKU_WIN_HPP

#include <ncurses.h>

#include "windows/interactiveWin.hpp"

#include "sudoku.hpp"

class SudokuWin: public InteractiveWin {
private:
    WINDOW *m_win;
    int m_curCell { 0 };
    bool m_showCurCell { true };
    Sudoku displayedSudoku;

private:
    void onArrowPress(int code);
    void updateHighlightedCell();
    void drawBorders();
    void drawSudoku();

    void selectNextCell();
    void selectPrevCell();

public:
    void init(const Sudoku &sudoku);

    void onKey(int code); 
    void focus();
    void blur();

    Sudoku getDisplayedSudoku();
    void setDisplayedSudoku(Sudoku sudoku);
};

#endif