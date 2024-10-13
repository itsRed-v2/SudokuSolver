#ifndef BUTTONS_WIN_HPP
#define BUTTONS_WIN_HPP

#include <ncurses.h>
#include <string>
#include <functional>

#include "interactiveWin.hpp"

#include "sudoku.hpp"

using namespace std;

struct Button {
    string id;
    string text;
    int y;
    int x;
    int colorPair;
    void(*callback)();
};

class ButtonsWin: public InteractiveWin {
private:
    WINDOW *m_win;
    int m_selectedButton { 0 };
    array<Button, 2> m_buttons;

private:
    void updateHighlightedButton();

public:
    void init();
    void setButtonCallback(const string &buttonId, void(*cb)());

    void onKey(int code);
    void focus();
    void blur();
};

#endif