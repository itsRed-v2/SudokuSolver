#ifndef FOCUSABLE_WIN_HPP
#define FOCUSABLE_WIN_HPP

#include <ncurses.h>

class InteractiveWin {
public:
    explicit InteractiveWin() = default;
    virtual void onKey(int code) = 0;
    virtual void focus() = 0;
    virtual void blur() = 0;
};


#endif