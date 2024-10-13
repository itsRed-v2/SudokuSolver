#ifndef SUDOKU_WIN_HPP
#define SUDOKU_WIN_HPP

#include <ncurses.h>
#include "sudoku.hpp"

class SudokuWin {
private:
    WINDOW *m_win;
    int m_curCell { 0 };
    int m_curCellSave { 0 };
    Sudoku displayedSudoku;

private:
    void updateHighlightedCell();
    void drawBorders();
    void drawSudoku();

public:
    explicit SudokuWin() = default;
    void init(const Sudoku &sudoku);
    void onArrowPress(int code);
    void selectNextCell();
    void onKey(int code); 
    void focus();
    void blur();

    Sudoku getDisplayedSudoku();
    void setDisplayedSudoku(Sudoku sudoku);
};

#endif