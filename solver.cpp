#include <iostream>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#include "solver.hpp"

#define sleep(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

// class CellState

void CellState::collapseTo(char state) {
    if (!this->hasState(state)) {
        throw runtime_error("Cannot collapse to a an impossible state");
    }

    for (int i = 0; i < 9; i++) {
        if (m_states[i] != state) {
            m_states[i] = 0;
        }
    }
}

bool CellState::hasState(char state) const {
    return m_states[state - 1] != 0;
}

int CellState::countStates() const {
    return count_if(
        m_states.begin(), 
        m_states.end(), 
        [](char state) -> bool { return state != 0; }
    );
}

char CellState::getValue() const {
    int stateCount = 0;
    char value;
    for (int i = 0; i < 9; i++) {
        if (m_states[i] != 0) {
            value = m_states[i];
            stateCount++;
        }
    }
    return stateCount == 1 ? value : 0;
}

void CellState::removeState(char state) {
    m_states[state - 1] = 0;
}

bool operator==(const CellState &lhs, const CellState &rhs) {
    return lhs.m_states == rhs.m_states;
}

bool operator!=(const CellState &lhs, const CellState &rhs) {
    return !(lhs == rhs);
}

// class CellGroup

void CellGroup::collapse() {
    collapseByCollision();
    collapseByUniqueness();
}

void CellGroup::collapseByCollision() {
    for (int s = 1; s <= 9; s++) {
        int index = this->getIndexOfCellWithNumber(s);
        if (index != -1) {
            for (int i = 0; i < 9; i++) {
                if (i != index) {
                    cells[i]->removeState(s);
                }
            }
        }
    }
}

void CellGroup::collapseByUniqueness() {
    for (int s = 1; s <= 9; s++) {
        auto predicate { [s](CellState* cell) -> bool { return cell->hasState(s); }};

        int candidateCount { static_cast<int>(count_if(
            cells.begin(), 
            cells.end(), 
            predicate
        )) };
        
        if (candidateCount == 1) {
            CellState** candidatePtr { find_if(cells.begin(), cells.end(), predicate) };
            (**candidatePtr).collapseTo(s);
        }
    }
}

int CellGroup::getIndexOfCellWithNumber(char number) const {
    for (int i = 0; i < 9; i++) {
        if (cells[i]->getValue() == number) {
            return i;
        }
    }
    return -1;
}

// class StateGrid

StateGrid::StateGrid(char sudokuGrid[9][9]) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (sudokuGrid[y][x] != 0) {
                m_grid[y][x].collapseTo(sudokuGrid[y][x]);
            }
        }
    }

    initializeGroups();
}

StateGrid::StateGrid(StateGrid &other) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            m_grid[y][x] = other.m_grid[y][x];
        }
    }

    initializeGroups();
}

void StateGrid::initializeGroups() {
    int groupIndex = 0;

    for (int row = 0; row < 9; row++) {
        CellGroup group;
        for (int x = 0; x < 9; x++) {
            group.cells[x] = &m_grid[row][x];
        }
        m_groups[groupIndex++] = group;
    }

    for (int col = 0; col < 9; col++) {
        CellGroup group;
        for (int y = 0; y < 9; y++) {
            group.cells[y] = &m_grid[y][col];
        }
        m_groups[groupIndex++] = group;
    }

    for (int squareY = 0; squareY < 3; squareY++) {
        for (int squareX = 0; squareX < 3; squareX++) {
            CellGroup group;
            for (int i = 0; i < 9; i++) {
                int y = squareY * 3 + i / 3;
                int x = squareX * 3 + i % 3;
                group.cells[i] = &m_grid[y][x];
            }
            m_groups[groupIndex++] = group;
        }
    }
}

void StateGrid::draw() const {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int cellValue = m_grid[y][x].getValue();
            char cellChar = cellValue == 0 ? '.' : cellValue + '0';
            cout << cellChar << " ";
            if (x == 2 || x == 5) {
                cout << "│ ";
            }
        }
        cout << endl;
        if (y == 2 || y == 5) {
            cout << "──────┼───────┼──────" << endl;
        }
    }
}

bool StateGrid::isSolved() const {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (m_grid[y][x].getValue() == 0) {
                return false;
            }
        }
    }
    return true;
}

bool StateGrid::isImpossible() const {
        for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (m_grid[y][x].countStates() == 0) {
                return true;
            }
        }
    }
    return false;
}

void StateGrid::collapseOnce() {
    for (CellGroup &group : m_groups) {
        group.collapse();
    }
}

void StateGrid::collapseFully() {
    StateGrid copy { *this };

    for (int i = 0;; i++) {
        this->collapseOnce();
        if (copy == *this) break;
        copy = *this;

        if (i == 100) {
            throw runtime_error("Stuck in infinite collapsing loop.");
        }
    }
}

CellState& StateGrid::getFewestStateCell() {
    CellState *fewestStateCell { nullptr };
    int fewestStateCount { 10 };
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int stateCount { m_grid[y][x].countStates() };
            if (stateCount > 1 && stateCount < fewestStateCount) {
                fewestStateCell = &m_grid[y][x];
                fewestStateCount = fewestStateCell->countStates();
            }
        }
    }

    if (fewestStateCell == nullptr) {
        throw runtime_error("Grid fully collapsed: no cell with more than 1 state.");
    }
    
    return *fewestStateCell;
}

bool operator==(const StateGrid &lhs, const StateGrid &rhs) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            if (lhs.m_grid[y][x] != rhs.m_grid[y][x] )
                return false;
        }
    }
    return true;
}

// functions

void SudokuSolver::solve(char grid[9][9]) {
    StateGrid states (grid);
    
    cout << "Grid:" << endl;
    states.draw();
    sleep(1000);

    solveRecursive(states);

    cout << "Final grid states:" << endl;
    states.draw();
}

void SudokuSolver::solveRecursive(StateGrid &states) {
    states.collapseFully();

    if (states.isSolved() || states.isImpossible()) {
        return;
    }

    CellState &fewestStateCell = states.getFewestStateCell();

    for (int s = 1; s <= 9; s++) {
        if (!fewestStateCell.hasState(s)) continue;

        StateGrid statesCopy { states };
        statesCopy.getFewestStateCell().collapseTo(s);
        solveRecursive(statesCopy);

        if (statesCopy.isSolved()) {
            states = statesCopy;
            return;
        }
    }
}