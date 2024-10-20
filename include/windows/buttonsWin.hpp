#ifndef BUTTONS_WIN_HPP
#define BUTTONS_WIN_HPP

#include <ncurses.h>
#include <string>
#include <algorithm>

#include "interactiveWin.hpp"

#include "sudoku.hpp"

using namespace std;

struct Button {
    string id;
    string text;
    int colorPair;
    void(*callback)();
};

class ButtonsWin: public InteractiveWin {
private:
    WINDOW *m_win;
    int m_selectedButton { 0 };
    array<Button, 4> m_buttons;
    bool m_focused = false;

private:
    void updateHighlightedButton();
    void redraw();
    int getButtonOffset(int buttonIndex);

public:
    void init(int y, int x);
    void setButtonCallback(const string &id, void(*cb)());
    void setButtonText(const string &id, string text);

    void onKey(int code);
    void focus();
    void blur();
};

#endif