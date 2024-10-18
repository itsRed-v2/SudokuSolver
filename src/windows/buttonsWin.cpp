#include <array>
#include <stdexcept>
#include "colors.hpp"

#include "windows/buttonsWin.hpp"

void ButtonsWin::init() {
    int height = 3, width = 28, startY = 0, startX = 25;
    m_win = newwin(height, width, startY, startX);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 2, "Menu");

    m_buttons = {
        Button{ "solve", "Solve !", 1, 3, CP_GREEN, NULL },
        Button{ "clear", "Clear", 1, 13, CP_RED, NULL },
        Button{ "exit", "Exit", 1, 21, CP_BLUE, NULL }
    };

    for (const Button &bt : m_buttons) {
        wattron(m_win, COLOR_PAIR(bt.colorPair));
        mvwaddstr(m_win, bt.y, bt.x, bt.text.c_str());
    }
    standend();

    wrefresh(m_win);
}

void ButtonsWin::onKey(int code) {
    if (code == KEY_LEFT && m_selectedButton > 0) {
        m_selectedButton--;
    } else if (code == KEY_RIGHT && m_selectedButton < m_buttons.size() - 1) {
        m_selectedButton++;
    } else if (code == '\n') {
        if (m_buttons[m_selectedButton].callback) {
            m_buttons[m_selectedButton].callback();
        }
    }

    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::updateHighlightedButton() {
    static int prevSelectedButton { -1 };
    if (prevSelectedButton != -1) {
        const Button &bt { m_buttons[prevSelectedButton] };
        mvwchgat(m_win, bt.y, bt.x, bt.text.length(), A_NORMAL, bt.colorPair, NULL);
    }
    if (m_selectedButton != -1) {
        const Button &bt { m_buttons[m_selectedButton] };
        mvwchgat(m_win, bt.y, bt.x, bt.text.length(), A_STANDOUT, bt.colorPair, NULL);
    }
    prevSelectedButton = m_selectedButton;
}

void ButtonsWin::focus() {
    mvwchgat(m_win, 0, 2, 4, A_BOLD, CP_BLUE, NULL);
    m_selectedButton = 0;
    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::blur() {
    mvwchgat(m_win, 0, 2, 4, A_NORMAL, CP_NORMAL, NULL);
    m_selectedButton = -1;
    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::setButtonCallback(const string &buttonId, void(*cb)()) {
    for (Button &bt : m_buttons) {
        if (bt.id == buttonId) {
            bt.callback = cb;
            return;
        }
    }
    throw runtime_error("Unknown id: " + buttonId);
}