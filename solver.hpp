#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <array>

using namespace std;

class CellState {
    array<char, 9> m_states { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

public:
    char getValue() const;
    bool hasState(char state) const;
    int countStates() const;

    void collapseTo(char state);
    void removeState(char state);

    friend bool operator==(const CellState &lhs, const CellState &rhs);
    friend bool operator!=(const CellState &lhs, const CellState &rhs);
};

class CellGroup {
public:
    array<CellState*, 9> cells;

    CellGroup() = default;
    void collapse();

private:
    void collapseByCollision();
    void collapseByUniqueness();
    int getIndexOfCellWithNumber(char number) const;
};

class StateGrid {
private:
    CellState m_grid[9][9];
    // A "group" represents a line, column or square of the sudoku grid.
    // Each group consists in pointers to the states of the 9 cells it contains.
    array<CellGroup, 27> m_groups;

    void initializeGroups();

public:
    StateGrid(char sudokuGrid[9][9]); // Standard constructor
    StateGrid(StateGrid &other); // Copy constructor
    void draw() const;
    bool isSolved() const;
    bool isImpossible() const;

    void collapseOnce();
    void collapseFully();
    CellState& getFewestStateCell() ;

    friend bool operator==(const StateGrid &lhs, const StateGrid &rhs);
};

class SudokuSolver {
public:
    SudokuSolver() = default;
    void solve(char grid[9][9]);

private:
    void solveRecursive(StateGrid &states);
};

#endif