#include "colors.hpp"

void initNcursesColors() {
    start_color();
    
    init_color(COLOR_GRAY, 75, 75, 75);
    init_color(COLOR_BLUE, 520, 600, 800);
    init_color(COLOR_GREEN, 375, 700, 400);
    init_color(COLOR_RED, 800, 300, 300);

    init_pair(CP_NORMAL, COLOR_WHITE, BACKGROUND_COLOR);
    init_pair(CP_BLUE, COLOR_BLUE, BACKGROUND_COLOR);
    init_pair(CP_GREEN, COLOR_GREEN, BACKGROUND_COLOR);
    init_pair(CP_RED, COLOR_RED, BACKGROUND_COLOR);

    bkgd(COLOR_PAIR(CP_NORMAL)); // Set background of stdscr
}