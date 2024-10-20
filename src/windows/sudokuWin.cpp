#include "colors.hpp"

#include "windows/sudokuWin.hpp"

#define xScreenPosOfCell(x) (2*x + (x / 3)*2 + 2)
#define yScreenPosOfCell(y) (y + (y/3) + 1)

int getX(int cellIndex) {
    return cellIndex % 9;
}

int getY(int cellIndex) {
    return cellIndex / 9;
}

bool isKeyPress(int code) {
    return code == KEY_LEFT || code == KEY_RIGHT || code == KEY_UP || code == KEY_DOWN;
}

void SudokuWin::init(int y, int x, const Sudoku &sudoku, string title) {
    m_title = title;
    m_win = newwin(13, 25, y, x);
    wbkgd(m_win, COLOR_PAIR(CP_NORMAL)); // Setting background color
    drawBorders(); // Drawing sudoku borders
    updateHighlightedCell(); // Highlight top left cell
    displayedSudoku = sudoku; // Initialize sudoku with default sudoku
    drawSudoku(); // Draw sudoku content
    mvwaddstr(m_win, 0, 2, m_title.c_str());
    wrefresh(m_win); // Print to screen the window buffer
}

void SudokuWin::onArrowPress(int code) {
    if (code == KEY_LEFT) m_curCell--;
    else if (code == KEY_RIGHT) m_curCell++;
    else if (code == KEY_UP && getY(m_curCell) > 0) m_curCell -= 9;
    else if (code == KEY_DOWN && getY(m_curCell) < 8) m_curCell += 9;

    if (m_curCell < 0) m_curCell = 0;
    if (m_curCell > 80) m_curCell = 80;

    updateHighlightedCell();
    wrefresh(m_win);
}

void SudokuWin::onKey(int code) {
    if (isKeyPress(code)) {
        onArrowPress(code);
    } else if (code >= '0' && code <= '9') {
        char value { static_cast<char>(code - '0') };
        displayedSudoku.cells[m_curCell] = value;
        drawSudoku();
        selectNextCell();
    } else if (code == KEY_BACKSPACE) {
        displayedSudoku.cells[m_curCell] = 0;
        drawSudoku();
        selectPrevCell();
    } else if (code == '\n') {
        selectNextCell();
    }
    wrefresh(m_win);
}

void SudokuWin::selectNextCell() {
    m_curCell++;
    if (m_curCell > 80) m_curCell = 80;
    updateHighlightedCell();
}

void SudokuWin::selectPrevCell() {
    m_curCell--;
    if (m_curCell < 0) m_curCell = 0;
    updateHighlightedCell();
}

void SudokuWin::updateHighlightedCell() {
    static int prevCell { -1 };
    
    if (prevCell >= 0 && prevCell < 81)
        // 1 is the number of chars to modify
        mvwchgat(m_win, yScreenPosOfCell(getY(prevCell)), xScreenPosOfCell(getX(prevCell)), 1, A_NORMAL, CP_NORMAL, NULL);
    if (m_showCurCell && m_curCell >= 0 && m_curCell < 81)
        mvwchgat(m_win, yScreenPosOfCell(getY(m_curCell)), xScreenPosOfCell(getX(m_curCell)), 1, A_STANDOUT, CP_NORMAL, NULL);
    
    prevCell = m_curCell;
}

void SudokuWin::drawSudoku() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int cellValue = displayedSudoku.grid[y][x];
            char cellChar = cellValue == 0 ? '.' : cellValue + '0';

            mvwaddch(m_win, yScreenPosOfCell(y), xScreenPosOfCell(x), cellChar);
        }
    }

    updateHighlightedCell();
}

void SudokuWin::focus() {
    // args: mvwchgat(window, start y, start x, nb of chars to change, attribute, color, NULL);
    mvwchgat(m_win, 0, 2, m_title.length(), A_BOLD, CP_BLUE, NULL);
    m_showCurCell = true;
    updateHighlightedCell();
    wrefresh(m_win);
}

void SudokuWin::blur() {
    // args: mvwchgat(window, start y, start x, nb of chars to change, attribute, color, NULL);
    mvwchgat(m_win, 0, 2, m_title.length(), A_NORMAL, CP_NORMAL, NULL);
    m_showCurCell = false;
    updateHighlightedCell();
    wrefresh(m_win);
}

Sudoku SudokuWin::getDisplayedSudoku() {
    return displayedSudoku;
}

void SudokuWin::setDisplayedSudoku(Sudoku sudoku) {
    displayedSudoku = sudoku;
    drawSudoku();
    wrefresh(m_win);
}

void SudokuWin::drawBorders() {
    for (int x = 0; x < 25; x++) {
        for (int y = 0; y < 13; y++) {
            if (x % 8 == 0) {
                mvwaddch(m_win, y, x, ACS_VLINE);
            }
            else if (y % 4 == 0) {
                mvwaddch(m_win, y, x, ACS_HLINE);
            }
        }
    }

    // Corners
    mvwaddch(m_win, 0, 0, ACS_ULCORNER);
    mvwaddch(m_win, 0, 24, ACS_URCORNER);
    mvwaddch(m_win, 12, 0, ACS_LLCORNER);
    mvwaddch(m_win, 12, 24, ACS_LRCORNER);

    // Top tees
    mvwaddch(m_win, 0, 8, ACS_TTEE);
    mvwaddch(m_win, 0, 16, ACS_TTEE);
    // Bottom tees
    mvwaddch(m_win, 12, 8, ACS_BTEE);
    mvwaddch(m_win, 12, 16, ACS_BTEE);
    // Left tees
    mvwaddch(m_win, 4, 0, ACS_LTEE);
    mvwaddch(m_win, 8, 0, ACS_LTEE);
    // Right tees
    mvwaddch(m_win, 4, 24, ACS_RTEE);
    mvwaddch(m_win, 8, 24, ACS_RTEE);

    // Center crosses
    mvwaddch(m_win, 4, 8, ACS_PLUS);
    mvwaddch(m_win, 4, 16, ACS_PLUS);
    mvwaddch(m_win, 8, 8, ACS_PLUS);
    mvwaddch(m_win, 8, 16, ACS_PLUS);
}