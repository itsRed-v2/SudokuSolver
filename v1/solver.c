#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// #define PRINT_RECURSION_TREE

void startTimer();
long stopTimer();
char solve(char grille[9][9]);
void printState(char state[9]);
void draw(char grille[9][9]);
void printRecursionDepth();

int recursions = 0;
int collapses = 0;
int recursionDepth = 0;
struct timeval end, start;

int main(void) {
  char grille1[9][9] = {
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

  // grille blonde platine
  char grille2[9][9] = {
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

  // grille facile
  char grille3[9][9] = {
    {0,2,0,0,0,9,6,0,0},
    {5,8,0,0,6,2,0,0,0},
    {7,0,6,0,3,0,0,1,9},
    {4,7,2,0,0,0,5,6,0},
    {9,5,0,6,0,0,3,4,2},
    {0,3,8,0,0,4,1,0,7},
    {8,1,7,0,0,5,0,0,6},
    {3,0,0,7,0,0,0,0,1},
    {0,0,0,0,9,0,0,7,0}
  };


  #define grille grille1

  printf("Grille de depart:\n");
  draw(grille);

  startTimer();
  char isSolved = solve(grille);
  long delta_usec = stopTimer();
  
  if (isSolved) {
    printf("Grille résolue:\n");
    draw(grille);
    printf("La grille a été résolue en %d réductions et %d récursions.\n", collapses, recursions);
  } else {
    printf("La grille n'est pas résolue.\n");
  }
  printf("Temps de calcul: %.3f ms.\n", delta_usec / 1000.0);
  
  return 0;
}

// fonctions pour mesurer le temps
void startTimer() {
  gettimeofday(&start, NULL);
}

long stopTimer() {
  gettimeofday(&end, NULL);
  long usec = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
  return usec;
}

// logique de résolution de sudoku
char getNumber(char states[9][9][9], char x, char y) {
  char number = 0;
  char numberCount = 0;
  for (int i = 0; i < 9; i++) {
    if (states[y][x][i] != 0) {
      number = states[y][x][i];
      numberCount++;
    }
  }
  if (numberCount != 1)
    return 0;
  return number;
}

char isOnLine(char number, char states[9][9][9], char cellX, char cellY) {
  for (int x = 0; x < 9; x++) {
    if (x != cellX && getNumber(states, x, cellY) == number) {
      return 1;
    }    
  }
  return 0;
}

char isOnColumn(char number, char states[9][9][9], char cellX, char cellY) {
  for (int y = 0; y < 9; y++) {
    if (y != cellY && getNumber(states, cellX, y) == number) {
      return 1;
    }    
  }
  return 0;
}

char isInBlock(char number, char states[9][9][9], char cellX, char cellY) {
  char boxStartX = cellX - (cellX % 3);
  char boxStartY = cellY - (cellY % 3);

  for (int y = boxStartY; y < boxStartY + 3; y++) {
    for (int x = boxStartX; x < boxStartX + 3; x++) {
      if (y == cellY && x == cellX)
        continue;
      if (getNumber(states, x, y) == number) {
        return 1;
      }
    }
  }
  return 0;
}

char isPossible(char number, char states[9][9][9], char cellX, char cellY) {
  return !isOnLine(number, states, cellX, cellY) && !isOnColumn(number, states, cellX, cellY) && !isInBlock(number, states, cellX, cellY);
}

char hasState(char number, char state[9]) {
  return state[number - 1] != 0;
}

char countStates(char state[9]) {
  char count = 0;
  for (int i = 0; i < 9; i++) {
    if (state[i] != 0) {
      count++;
    }
  }
  return count;
}

char getFewestStatesCell(char states[9][9][9]) {
  char fewestStates = 10;
  char cellX = 0;
  char cellY = 0;
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      char stateCount = countStates(states[y][x]);
      if (stateCount > 1 && stateCount < fewestStates) {
        fewestStates = stateCount;
        cellX = x;
        cellY = y;
      }
    }
  }
  return cellY * 9 + cellX;
}

void collapseStateTo(char number, char state[9]) {
  for (int i = 0; i < 9; i++) {
    if (state[i] != number) {
      state[i] = 0;
    }
  }
}

void collapseLineByUniqueness(char states[9][9][9], char line, char stateToKeep) {
  char stateCount = 0;
  for (int x = 0; x < 9; x++) {
    if (hasState(stateToKeep, states[line][x])) {
      stateCount++;
    }
  }

  if (stateCount != 1)
    return;

  for (int x = 0; x < 9; x++) {
    if (hasState(stateToKeep, states[line][x])) {
      collapseStateTo(stateToKeep, states[line][x]);
    }
  }
}

void collapseColByUniqueness(char states[9][9][9], char column, char stateToKeep) {
  char stateCount = 0;
  for (int y = 0; y < 9; y++) {
    if (hasState(stateToKeep, states[y][column])) {
      stateCount++;
    }
  }

  if (stateCount != 1)
    return;

  for (int y = 0; y < 9; y++) {
    if (hasState(stateToKeep, states[y][column])) {
      collapseStateTo(stateToKeep, states[y][column]);
    }
  }
}

void collapseBlockByUniqueness(char states[9][9][9], char blockX, char blockY, char stateToKeep) {
  char stateCount = 0;
  
  for (int y = blockY; y < blockY + 3; y++) {
    for (int x = blockX; x < blockX + 3; x++) {
      if (hasState(stateToKeep, states[y][x])) {
        stateCount++;
      }
    }
  }

  if (stateCount != 1)
    return;

  for (int y = blockY; y < blockY + 3; y++) {
    for (int x = blockX; x < blockX + 3; x++) {
      if (hasState(stateToKeep, states[y][x])) {
        collapseStateTo(stateToKeep, states[y][x]);
      }
    }
  }
}

void collapseByUniqueness(char states[9][9][9], char stateToKeep) {
  for (int i = 0; i < 9; i++) {
    collapseLineByUniqueness(states, i, stateToKeep);
    collapseColByUniqueness(states, i, stateToKeep);
  }

  for (int y = 0; y < 9; y += 3) {
    for (int x = 0; x < 9; x += 3) {
      collapseBlockByUniqueness(
        states, x, y, stateToKeep
      );
    }
  }
}

void collapse(char states[9][9][9]) {
  collapses++;
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) { 
      for (int s = 0; s < 9; s++) {
        if (states[y][x][s] != 0 && !isPossible(states[y][x][s], states, x, y)) {
          states[y][x][s] = 0;
        }
      }
    }
  }

  for (int i = 1; i <= 9; i++)
    collapseByUniqueness(states, i);
}

void copyStates(char source[9][9][9], char destination[9][9][9]) {
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) { 
      for (int s = 0; s < 9; s++) {
        destination[y][x][s] = source[y][x][s];
      }
    }
  }
}

char statesEqual(char states1[9][9][9], char states2[9][9][9]) {
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) { 
      for (int s = 0; s < 9; s++) {
        if (states1[y][x][s] != states2[y][x][s]) {
          return 0;
        }
      }
    }
  }
  return 1;
}

void collapseCompletely(char states[9][9][9]) {
  char oldStates[9][9][9];
  for (int i = 1; i < 20; i++) {
    copyStates(states, oldStates);
    collapse(states);

    if (statesEqual(states, oldStates)) {
      break;
    }
  }
}


char isGridImpossible(char states[9][9][9]) {
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      if (countStates(states[y][x]) == 0) {
        return 1;
      }
    }
  }
  return 0;
  // todo: grille impossible si un chiffre n'est possible dans aucune case d'une ligne / colonne / bloc
}

char isGridComplete(char states[9][9][9]) {
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      if (countStates(states[y][x]) != 1) {
        return 0;
      }
    }
  }
  return 1;
}

void stateToGrid(char states[9][9][9], char grille[9][9]) {
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      grille[y][x] = getNumber(states, x, y);
    }
  }
}

char solveRecursive(char states[9][9][9]) {
  recursions++;
  recursionDepth++;
  #ifdef PRINT_RECURSION_TREE
  printRecursionDepth();
  #endif
  
  collapseCompletely(states);

  if (isGridImpossible(states)) {
    recursionDepth--;
    return 0;
  }

  if (isGridComplete(states)) {
    recursionDepth--;
    return 1;
  }

  char fewestStateCellPos = getFewestStatesCell(states);
  char cellX = fewestStateCellPos % 9;
  char cellY = fewestStateCellPos / 9;

  for (int i = 1; i <= 9; i++) {    
    if (!hasState(i, states[cellY][cellX])) continue;

    char statescp[9][9][9];
    copyStates(states, statescp);
    
    collapseStateTo(i, statescp[cellY][cellX]);
    if (solveRecursive(statescp)) {
      copyStates(statescp, states);
      recursionDepth--;
      return 1;
    }
  }

  recursionDepth--;
  return 0;
}

char solve(char grille[9][9]) {
  char states[9][9][9];
  for (int x = 0; x < 9; x++) {
    for (int y = 0; y < 9; y++) { 
      for (int s = 0; s < 9; s++) {
        if (grille[y][x] == 0) {
          states[y][x][s] = s + 1;
        } else {
          states[y][x][s] = (s + 1 == grille[y][x]) ? s + 1 : 0;
        }
      }
    }
  }

  char isSolved = solveRecursive(states);
  if (isSolved) {
    stateToGrid(states, grille);
  }
  return isSolved;
}

// code pour afficher la grille de sudoku
void printState(char state[9]) {
  for (int i = 0; i < 9; i++) {
    if (state[i] != 0) {
      printf("%d ", state[i]);
    }
  }
  printf("\n");
}

void replaceChar(char *str, char find, char replace) {
  for (int i=0; str[i] != '\0'; i++) {
    if (str[i] == find) {
      str[i] = replace;
    }
  }
}

void printLigne(char row[9]) {
  char* ligne = malloc(26 * sizeof(char));
  char* format = "%d %d %d │ %d %d %d │ %d %d %d";
  sprintf(ligne, format, row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
  replaceChar(ligne, '0', '.');
  printf("%s\n", ligne);
}

void draw(char grille[9][9]) {
  for (int y = 0; y < 9; y++) {
    printLigne(grille[y]);
    if (y == 2 || y == 5) {
      printf("──────┼───────┼──────\n");
    }
  }
}

void printRecursionDepth() {
  for (int i = 0; i < recursionDepth; i++) {
    if (i < recursionDepth - 2)
      printf("│");
    else if (i == recursionDepth - 2)
      printf("├");
    else
      printf("┐");
  }
  printf("\n");
}