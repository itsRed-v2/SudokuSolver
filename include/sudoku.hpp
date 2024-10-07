#ifndef SUDOKU_HPP
#define SUDOKU_HPP

union Sudoku {
    char cells[81];
    char grid[9][9];
};

#endif