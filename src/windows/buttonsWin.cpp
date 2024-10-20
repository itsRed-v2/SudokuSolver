#include <array>
#include <stdexcept>
#include "colors.hpp"

#include "windows/buttonsWin.hpp"

void ButtonsWin::init(int y, int x) {
    m_buttons = {
        Button{ "solve", "Solve !", CP_GREEN, NULL },
        Button{ "clear", "Clear", CP_RED, NULL },
        Button{ "toggle_animation", "Enable animation ", CP_NORMAL, NULL },
        Button{ "exit", "Exit", CP_BLUE, NULL }
    };

    int height = 3, width = 75;
    m_win = newwin(height, width, y, x);
    wbkgd(m_win, COLOR_PAIR(CP_NORMAL)); // Setting background color

    redraw();
}

void ButtonsWin::redraw() {
    wclear(m_win);
    box(m_win, 0, 0);
    mvwprintw(m_win, 0, 2, "Menu");
    // Window title formatting
    if (m_focused)
        mvwchgat(m_win, 0, 2, 4, A_BOLD, CP_BLUE, NULL);

    // Draw buttons
    int offsetX = 3;
    for (const Button &bt : m_buttons) {
        wattron(m_win, COLOR_PAIR(bt.colorPair));
        mvwaddstr(m_win, 1, offsetX, bt.text.c_str());
        offsetX += bt.text.length() + 3;
    }
    wstandend(m_win);

    wrefresh(m_win);
}

int ButtonsWin::getButtonOffset(int buttonIndex) {
    int offsetX = 3;
    for (int i = 0; i < buttonIndex; i++) {
        offsetX += m_buttons[i].text.length() + 3;
    }
    return offsetX;
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
        mvwchgat(m_win, 1, getButtonOffset(prevSelectedButton), bt.text.length(), A_NORMAL, bt.colorPair, NULL);
    }
    if (m_selectedButton != -1) {
        const Button &bt { m_buttons[m_selectedButton] };
        mvwchgat(m_win, 1, getButtonOffset(m_selectedButton), bt.text.length(), A_STANDOUT, bt.colorPair, NULL);
    }
    prevSelectedButton = m_selectedButton;
}

void ButtonsWin::focus() {
    m_focused = true;
    mvwchgat(m_win, 0, 2, 4, A_BOLD, CP_BLUE, NULL);
    m_selectedButton = 0;
    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::blur() {
    m_focused = false;
    mvwchgat(m_win, 0, 2, 4, A_NORMAL, CP_NORMAL, NULL);
    m_selectedButton = -1;
    updateHighlightedButton();
    wrefresh(m_win);
}

void ButtonsWin::setButtonCallback(const string &id, void(*cb)()) {
    for (Button &bt : m_buttons) {
        if (bt.id == id) {
            bt.callback = cb;
            return;
        }
    }
    throw runtime_error("Unknown button id: " + id);
}

void ButtonsWin::setButtonText(const string &id, string text) {
    for (Button &bt : m_buttons) {
        if (bt.id == id) {
            bt.text = text;
            redraw();
            return;
        }
    }
    throw runtime_error("Unknown button id: " + id);
}