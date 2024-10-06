#include <iostream>
#include <string>
#include <algorithm>
#include <iostream>

#include "solver.hpp"

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
    if (state < 1 || state > 9) throw runtime_error("Illegal state value");
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

CellGroup::CellGroup(array<CellState*, 9> cells) : m_cells(cells) {

}

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
                    m_cells[i]->removeState(s);
                }
            }
        }
    }
}

void CellGroup::collapseByUniqueness() {
    for (int s = 1; s <= 9; s++) {
        auto predicate { [s](CellState* cell) -> bool { return cell->hasState(s); }};

        int candidateCount { static_cast<int>(count_if(
            m_cells.begin(), 
            m_cells.end(), 
            predicate
        )) };
        
        if (candidateCount == 1) {
            CellState** candidatePtr { find_if(m_cells.begin(), m_cells.end(), predicate) };
            (**candidatePtr).collapseTo(s);
        }
    }
}

int CellGroup::getIndexOfCellWithNumber(char number) const {
    for (int i = 0; i < 9; i++) {
        if (m_cells[i]->getValue() == number) {
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
    for (int row = 0; row < 9; row++) {
        array<CellState*, 9> cells;
        for (int x = 0; x < 9; x++) {
            cells[x] = &m_grid[row][x];
        }
        m_groups.push_back(CellGroup{cells});
    }

    for (int col = 0; col < 9; col++) {
        array<CellState*, 9> cells;
        for (int y = 0; y < 9; y++) {
            cells[y] = &m_grid[y][col];
        }
        m_groups.push_back(CellGroup{cells});
    }

    for (int squareY = 0; squareY < 3; squareY++) {
        for (int squareX = 0; squareX < 3; squareX++) {
            array<CellState*, 9> cells;
            for (int i = 0; i < 9; i++) {
                int y = squareY * 3 + i / 3;
                int x = squareX * 3 + i % 3;
                cells[i] = &m_grid[y][x];
            }
            m_groups.push_back(CellGroup{cells});
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
    solveRecursive(states);

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