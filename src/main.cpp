#include "solver.hpp"

#include <iostream>

using namespace std;

void draw(const StateGrid &grid) {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            int cellValue = grid.valueAt(x, y);
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

int main() {
    char grid[9][9] = {
        {7,3,5,0,0,0,1,9,6},
        {0,0,9,1,7,5,8,0,2},
        {0,0,1,0,6,9,7,0,5},
        {9,2,0,0,5,0,0,7,8},
        {0,5,0,0,0,0,0,2,0},
        {3,0,0,0,0,0,5,0,0},
        {0,0,0,5,0,0,0,1,0},
        {0,9,4,0,0,0,2,0,0},
        {0,0,0,6,0,7,0,0,4}
    };

    char blondePlatine[9][9] = {
        {0,0,0,0,0,0,0,1,2},
        {0,0,0,0,0,0,0,0,3},
        {0,0,2,3,0,0,4,0,0},
        {0,0,1,8,0,0,0,0,5},
        {0,6,0,0,7,0,8,0,0},
        {0,0,0,0,0,9,0,0,0},
        {0,0,8,5,0,0,0,0,0},
        {9,0,0,0,4,0,5,0,0},
        {4,7,0,0,0,6,0,0,0}
    };

    char impossible[9][9] = {
        {0,0,6,0,0,0,7,1,2},
        {0,0,9,0,0,0,0,0,3},
        {0,0,2,3,0,0,4,0,0},
        {0,9,1,8,0,0,0,0,5},
        {0,6,4,0,7,0,8,0,0},
        {0,0,7,0,0,9,0,0,0},
        {6,0,8,5,0,0,0,0,0},
        {9,0,3,0,4,0,5,0,0},
        {4,7,5,0,0,6,0,0,0}
    };

    char volodia[9][9] = {
        {0,3,0,6,0,2,9,0,0},
        {0,2,0,5,0,9,0,0,0},
        {0,4,9,0,3,0,2,0,0},
        {0,9,0,0,0,7,0,3,2},
        {2,1,8,0,0,0,0,0,0},
        {0,0,0,2,0,0,0,9,6},
        {0,0,2,0,1,5,0,0,8},
        {1,0,0,0,0,0,4,2,0},
        {0,7,0,0,2,0,0,6,0}
    };

    SudokuSolver solver;

    cout << "Solving grid:" << endl;
    SudokuResult result { solver.solve(volodia) };

    draw(result.grid);
    cout << (result.isSolved ? "The sudoku is solved." : "Could not solve the sudoku.") << endl;
    cout << "Solving took " << (result.calculationTimeMicrosec / 1000.0) << " milliseconds." << endl;
}