#ifndef BUTTONS_WIN_HPP
#define BUTTONS_WIN_HPP

#include <ncurses.h>
#include <string>
#include <functional>

#include "sudoku.hpp"

using namespace std;

class ButtonsWin {
private:
    WINDOW *m_win;
    int m_selectedButton { 0 };

public:
    explicit ButtonsWin() = default;
    void init();
    void onKey(int code);

    void setButtonCallback(const string &buttonId, void(*cb)());

    void focus();
    void blur();

private:
    void updateHighlightedButton();
};

#endif