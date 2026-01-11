#include "c4lib.h"
#include <iostream>
using namespace std;

// Prototype internal helper functions
/**
 * @brief returns the smallest y coordinate that is BLANK in 
 *        column x or -1 if no location is BLANK
 */
int findYValue(BoardValue** board, int ydim, int x);


// ------------------------------------------------------------
// Global variabes - the ONLY we allow in this program

// converts integer player value: 0 or 1 to appropriate enum
enum BoardValue playerToValue[2] = {RED, YELLOW};

// ----------------------------------------------------------------------


// Helper function 
int findYValue(BoardValue** board, int ydim,  int x)
{
  // You complete!
  if (x >= ydim || x < 0) {
    return -1;
  }

  for (int i = 0; i < ydim; i++) {
    if (board[i][x] == BLANK) { // BLANK = 0;
      return i;
    }
  }
  return -1; // returns -1 if the column is full 
}

// Allocates memory for the game board:
BoardValue** allocateBoard(int ydim,  int xdim)
{
  // You complete!
  // REMINDS : board [y][x]
  
  // // VALGRIND:
  // if (ydim <= 0 || xdim <= 0) {
  //   return nullptr;
  // }

  BoardValue** board = new BoardValue*[ydim];

  // // VALGRIND:
  // if (!board) {
  //   return nullptr;
  // }

  // adds to rows
  for (int i = 0; i < ydim; i++) {
    board[i] = new BoardValue[xdim];
    // adds to columns

    // // VALGRIND:
    // if (!board[i]) {
    //   return nullptr; 
    // }

    for (int k = 0; k < xdim; k++) {
      board[i][k] = BLANK; // 0
    } 
  }

  return board;
}

// Deallocate memory for the game board 
void deallocateBoard(BoardValue** board,  int ydim)
{
  // You complete!
  for (int i = 0; i < ydim; i++) {
    delete [] board[i];
  }
  
  delete [] board; 
}

void printBoard(BoardValue** board,  int ydim, int xdim)
{
  // Complete - Do not alter
  const char* boardToString[] = { "\U000026ab", "\U0001F534", "\U0001F7E1" }; 
                                  //  "⚫",          "🔴",         "🟡"}
  for(int y = ydim-1; y >= 0; y--) {
    for(int x = 0; x < xdim; x++) {
      cout << boardToString[(int)board[y][x]] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

bool getNextHumanInput(
  BoardValue** board, 
  int ydim,  int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  // You complete!

  // return true to indicate error.
  cout << "Enter the location: " << endl;
  cin >> *x;
  
  *y = findYValue(board, ydim, *x);

  if (*y == -1) { // column is full
    return true;
  }

  if (*x < 0 || *x >= xdim) { //invalid column index
    return true;
  }

  // Update board:
  board[*y][*x] = playerToValue[currentPlayer]; // places the correct color into the baord;
  return false;
}

bool hasWon(
  BoardValue** board,
  int ydim,  int xdim,
  int sy, int sx,
  int currentPlayer)
{
  // Keep, modify, or delete these as you desire
  const int NDIRS=4; // directions to check -> left, right, diagonal x2
  const int yDirDelta[NDIRS] = {+1,  0, +1, +1}; // change in ydir for each direction
  const int xDirDelta[NDIRS] = { 0, +1, -1, +1}; // change in xdir for each direction
  // You complete!

  // cout << "DEBUG: " << currentPlayer << "at: " << sy << sx << endl;

  // get BoardValue that corresponds to the current player (ie. red or yellow)
  BoardValue playerValue = playerToValue[currentPlayer];

  for (int i = 0; i < NDIRS; i++) {
    int count = 1;
    // checks forward direction 
    for (int k = 1; k < 4; k++) {
      int newY = sy + k * yDirDelta[i];
      int newX = sx + k * xDirDelta[i];
      
      // ensure position is within bounds:
      if (!(newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)) {
        break;
      }
      
      if (board[newY][newX] == playerValue) {
          count++;
      }
      else {
        break;
      }
    } // end of forward direction checking
  
  // checking backward direction
    for (int k = 1; k < 4; k++) {
      int newY = sy - k * yDirDelta[i];
      int newX = sx - k * xDirDelta[i];
      
      // has to be in bounds AND contain the player's piece.
      if (!(newX >= 0 && newX < xdim && newY >= 0 && newY < ydim)) {
        break;
      }
      if (board[newY][newX] == playerValue) {
        count++;
      }
      else {
        break;
      }  
    }
    
    // cout << "Direction: " << i << " count: " << count << endl;
    if (count >= 4) { // player wins
      return true; 
    }
  }
  return false;
}

bool isDraw(
  BoardValue** board,
  int ydim,  int xdim)
{
  // You complete!

  // draw = board is full;
  for (int i = 0; i < xdim; i++) {
    if (board[ydim - 1][i] == BLANK) { // if any are in the top row of the cell
      return false;
    }
  }
  return true;
}



bool getUserAIInput(
  BoardValue** board, 
  int ydim,  int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  // You complete!
  // Check if the current player can win
  BoardValue myAIValue = playerToValue[currentPlayer]; // stores AI color
  BoardValue opponent = playerToValue[1 - currentPlayer]; // stores the opponents color

  // Check if we need to block 
  //  We can greedily play the first blocking location since
  //  if they can win in multiple ways it does not matter which
  //  we choose.

  // CAN WIN:
  for (int c = 0; c < xdim; c++) {
    int r = findYValue(board, ydim, c);
    // theres space in the column
    if (r != -1) { 
      board[r][c] = myAIValue; // simulates a move

      if(hasWon(board, ydim, xdim, r, c, currentPlayer)) {
        *x = c;
        *y = r;
        return false; // play winning move
    }
    board[r][c] = BLANK; // undoes the simulation;
    }
  }

  // BLOCK:
  for (int c = 0; c < xdim; c++) {
  int r = findYValue(board, ydim, c);
    if (r != -1) {
      board[r][c] = opponent; // simulation of opponents move

      if (hasWon(board, ydim, xdim, r, c, 1 - currentPlayer)) {
        board[r][c] = BLANK;

        *x = c;
        *y = r;
        board[*y][*x] = myAIValue; // blocks the move 
        return false; //found a blocking move
    }
    board[r][c] = BLANK; // undoes simulation
    }
  }



  // Add any other logic for how to choose a location to play
  // if neither case above occurs

  // STRATEGIC:
  int bestChoice = -1;
  int max = 0;

  for (int c = 0; c < xdim; c++) {
    int yVal = findYValue(board, ydim, c);
    if (yVal != -1) {
      int count = 0;
      for (int r = 0; r < ydim; r++) {
        if (board[r][c] == myAIValue) {
          count++;
        }
      }

      if (count > max) {
        max = count; 
        bestChoice = c;
      }
    }
  }

  if (bestChoice != -1) {
    *x = bestChoice;
    *y = findYValue(board, ydim, bestChoice);
    board[*y][*x] = myAIValue;
    return false;
  }

// if none of the aboves run, the AI still has to make a move:
  for (int c = 0; c < xdim; c++) {
    int r = findYValue(board, ydim, c);
    if (r != -1) {
      *x = c;
      *y = r;
      board[*y][*x] = myAIValue;
      return false;
    }
  }

  return true; // no valid move;
}

// Complete - Do not alter
bool getRandomAIInput(
  BoardValue** board, 
  int ydim, int xdim, 
  int *y, int *x,
  int currentPlayer)
{
  // Allocate variable size array (we don't know xdim when we compile)
  int* possible = new int[xdim];
  int numX = 0;
  bool error = true;
  // find all the possible columns with a blank
  for(int i = 0; i < xdim; i++){
    if(board[ydim-1][i] == BLANK){
      possible[numX++] = i;
    }
  }
  // If at least 1 column has a blank
  if(numX != 0){
    // Choose one column
    *x = possible[rand()%numX];
    *y = findYValue(board, ydim, *x);
    board[*y][*x] = playerToValue[currentPlayer];
    error = false;  
  }
  delete [] possible;
  return error;
}


