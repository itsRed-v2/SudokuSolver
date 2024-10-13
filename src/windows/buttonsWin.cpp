#include <array>
#include <stdexcept>

#include "windows/buttonsWin.hpp"

struct Button {
    string id;
    string text;
    int y;
    int x;
    int colorPair;
    void(*callback)();
};

array<Button, 2> buttons = {
    Button{ "solve", "Solve !", 1, 3, 2 },
    Button{ "clear", "Clear", 1, 13, 3 }
};

void ButtonsWin::init() {
    m_win = newwin(3, 21, 0, 25);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 2, "Menu");

    for (const Button &bt : buttons) {
        wattron(m_win, COLOR_PAIR(bt.colorPair));
        mvwaddstr(m_win, bt.y, bt.x, bt.text.c_str());
    }
    standend();

    wrefresh(m_win);
}

void ButtonsWin::onKey(int code) {
    if (code == KEY_LEFT && m_selectedButton > 0) {
        m_selectedButton--;
    } else if (code == KEY_RIGHT && m_selectedButton < 1) {
        m_selectedButton++;
    } else if (code == '\n') {
        if (buttons[m_selectedButton].callback) {
            buttons[m_selectedButton].callback();
        }
    }

    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::updateHighlightedButton() {
    static int prevSelectedButton { -1 };
    if (prevSelectedButton != -1) {
        const Button &bt { buttons[prevSelectedButton] };
        mvwchgat(m_win, bt.y, bt.x, bt.text.length(), A_NORMAL, bt.colorPair, NULL);
    }
    if (m_selectedButton != -1) {
        const Button &bt { buttons[m_selectedButton] };
        mvwchgat(m_win, bt.y, bt.x, bt.text.length(), A_STANDOUT, bt.colorPair, NULL);
    }
    prevSelectedButton = m_selectedButton;
}

void ButtonsWin::focus() {
    mvwchgat(m_win, 0, 2, 4, A_BOLD, 1, NULL);
    m_selectedButton = 0;
    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::blur() {
    mvwchgat(m_win, 0, 2, 4, A_NORMAL, 0, NULL);
    m_selectedButton = -1;
    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::setButtonCallback(const string &buttonId, void(*cb)()) {
    for (Button &bt : buttons) {
        if (bt.id == buttonId) {
            bt.callback = cb;
            return;
        }
    }
    throw runtime_error("Unknown id: " + buttonId);
}