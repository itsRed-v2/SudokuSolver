#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <array>
#include <vector>
#include <chrono>
#include <string>
#include <unordered_map>

#include "sudoku.hpp"

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
private:
    array<CellState*, 9> m_cells;

    void collapseByCollision();
    void collapseByUniqueness();
    int getIndexOfCellWithNumber(char number) const;

public:
    CellGroup(array<CellState*, 9> cells);
    void collapse();
};

class StateGrid {
private:
    CellState m_grid[9][9];
    // A "group" represents a line, column or square of the sudoku grid.
    // Each group consists in pointers to the states of the 9 cells it contains.
    vector<CellGroup> m_groups;

    void initializeGroups();

public:
    StateGrid(const Sudoku &grid); // Standard constructor
    StateGrid(StateGrid &other); // Copy constructor
    bool isSolved() const;
    bool isImpossible() const;
    char valueAt(int x, int y) const;
    Sudoku toSudoku() const;

    void collapseOnce();
    void collapseFully();
    CellState& getFewestStateCell();

    friend bool operator==(const StateGrid &lhs, const StateGrid &rhs);
};

struct SolveInfo {
    Sudoku sudoku;
    bool isSolved = false;
    int64_t calculationTimeMicrosec = -1;
    unsigned int deadEndsFound = 0;
    unsigned int recursiveCalls = 0;
    unsigned int currentRecursionDepth = 0;
};

class SudokuSolver {
private:
    unordered_map<string, void(*)(const SolveInfo &curData)> m_callbacks;
public:
    bool runCallbacks = false;

public:
    SudokuSolver() = default;
    SolveInfo solve(const Sudoku &grid);
    void setCallback(const string &key, void(*cb)(const SolveInfo &curData));

private:
    void solveRecursive(StateGrid &states, SolveInfo &data);
    void runCallBack(const string &key, const SolveInfo &curData);
};

#endif