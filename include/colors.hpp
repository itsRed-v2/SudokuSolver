#ifndef SUDOKU_COLORS_HPP
#define SUDOKU_COLORS_HPP

#include <ncurses.h>

#define COLOR_GRAY 8 // Values 0 through 7 used by ncurses itself

#define CP_NORMAL 1
#define CP_BLUE 2
#define CP_GREEN 4
#define CP_RED 5
#define CP_GRAY 6

#define BACKGROUND_COLOR COLOR_GRAY

void initNcursesColors();

#endif