#include "windows/statsWin.hpp"

void StatsWin::init() {
    int height = 10, width = 28, startY = 3, startX = 25;
    m_win = newwin(height, width, startY, startX);
}

void StatsWin::displayStats(const SudokuResult &result) {
    wclear(m_win);
    wprintw(m_win, "Solving time: %.3f ms.", result.calculationTimeMicrosec / 1000.0);
    wrefresh(m_win);
}
