#ifndef INFO_WIN_HPP
#define INFO_WIN_HPP

#include <ncurses.h>
#include "solver.hpp"

class InfoWin {
private:
    WINDOW* m_win;

public:
    explicit InfoWin() = default;
    void init(int y, int x);
    void clear();
    void displayStats(const SolveInfo &result);
    void displayIntermediate(const SolveInfo &result);
    void sayImpossibleSudoku(const SolveInfo &result);
};

#endif