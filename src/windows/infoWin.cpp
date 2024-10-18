#include "colors.hpp"

#include "windows/infoWin.hpp"

void InfoWin::init() {
    int height = 10, width = 28, startY = 3, startX = 25;
    m_win = newwin(height, width, startY, startX);
}

void InfoWin::clear() {
    wclear(m_win);
    wrefresh(m_win);
}

void InfoWin::displayStats(const SudokuResult &result) {
    wclear(m_win);
    wprintw(m_win, "Solved in %.3f ms.", result.calculationTimeMicrosec / 1000.0);
    wrefresh(m_win);
}

void InfoWin::sayImpossibleSudoku(const SudokuResult &result) {
    wclear(m_win);
    wattron(m_win, COLOR_PAIR(CP_RED));
    wprintw(m_win, "This sudoku is unsolvable.\n");
    wstandend(m_win);
    wprintw(m_win, "Computed in %.3f ms.", result.calculationTimeMicrosec / 1000.0);
    wrefresh(m_win);
}