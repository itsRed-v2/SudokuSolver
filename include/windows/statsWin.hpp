#ifndef STATS_WIN_HPP
#define STATS_WIN_HPP

#include <ncurses.h>
#include "solver.hpp"

class StatsWin {
private:
    WINDOW* m_win;

public:
    explicit StatsWin() = default;
    void init();
    void displayStats(const SudokuResult &result);
};

#endif