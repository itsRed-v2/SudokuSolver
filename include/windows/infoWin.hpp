#ifndef INFO_WIN_HPP
#define INFO_WIN_HPP

#include <ncurses.h>
#include "solver.hpp"

class InfoWin {
private:
    WINDOW* m_win;

public:
    explicit InfoWin() = default;
    void init();
    void clear();
    void displayStats(const SudokuResult &result);
    void sayImpossibleSudoku(const SudokuResult &result);
};

#endif