#include "colors.hpp"

#include "windows/infoWin.hpp"

void InfoWin::init(int y, int x) {
    int height = 13, width = 25;
    m_win = newwin(height, width, y, x);
    wbkgd(m_win, COLOR_PAIR(CP_NORMAL)); // Setting background colorpov
    wrefresh(m_win);
}

void InfoWin::clear() {
    wclear(m_win);
    wrefresh(m_win);
}

void InfoWin::displayStats(const SolveInfo &result) {
    wclear(m_win);
    wprintw(m_win, "Solved in %.3f ms.\n", result.calculationTimeMicrosec / 1000.0);
    wprintw(m_win, "Recursive calls: %d\n", result.recursiveCalls);
    wprintw(m_win, "Dead ends: %d\n", result.deadEndsFound);
    wrefresh(m_win);
}

void InfoWin::displayIntermediate(const SolveInfo &result) {
    wclear(m_win);
    wprintw(m_win, "Computing...\n");
    wprintw(m_win, "Recursive calls: %d\n", result.recursiveCalls);
    wprintw(m_win, "Dead ends: %d\n", result.deadEndsFound);
    wprintw(m_win, "Recursion depth: %d\n", result.currentRecursionDepth);
    for (int i = 0; i < result.currentRecursionDepth; i++)
        waddch(m_win, '-');
    wrefresh(m_win);
}

void InfoWin::sayImpossibleSudoku(const SolveInfo &result) {
    wclear(m_win);
    wattron(m_win, COLOR_PAIR(CP_RED));
    wprintw(m_win, "This sudoku is unsolvable.\n");
    wstandend(m_win);
    wprintw(m_win, "Computed in %.3f ms.\n", result.calculationTimeMicrosec / 1000.0);
    wprintw(m_win, "Recursive calls: %d\n", result.recursiveCalls);
    wprintw(m_win, "Dead ends: %d\n", result.deadEndsFound);
    wrefresh(m_win);
}