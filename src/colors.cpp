#include "colors.hpp"

void initNcursesColors() {
    start_color();
    init_pair(CP_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(CP_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(CP_RED, COLOR_RED, COLOR_BLACK);
}