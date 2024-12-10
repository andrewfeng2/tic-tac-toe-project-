#ifndef GAME_STATE_H
#define GAME_STATE_H


#include <iostream>


// Vector structure to represent 2D coordinates
struct Vec {
   int x;
   int y;


   // Default constructor
   Vec() {
       x = 0;
       y = 0;
   }


   // Parameterized constructor
   Vec(int x, int y) {
       this->x = x;
       this->y = y;
   }


   // Set new coordinates
   void set(int x, int y) {
       this->x = x;
       this->y = y;
   }
};


// Overload << operator for Vec to allow easy printing
std::ostream& operator<<(std::ostream& os, const Vec& v) {
   os << "(" << v.x << ", " << v.y << ")";
   return os;
}


// Forward declarations
struct GameState;
std::ostream& operator<<(std::ostream& os, const GameState& state);


// Main GameState structure representing the Tic-Tac-Toe game
struct GameState {
   int** grid;         // 2D array to represent the game board
   bool currentTurn;   // Current player's turn (0 for X, 1 for O)
   int size;           // Size of the game board (e.g., 3 for 3x3)
   int turnCount;      // Number of turns played
   bool done;          // Flag to indicate if the game is over
   Vec lastMove;       // Coordinates of the last move played


   // Default constructor (3x3 board)
   GameState() {
       size = 3;
       currentTurn = 0;
       turnCount = 0;
       done = false;
       lastMove.set(-1, -1);
       grid = new int*[size];
       for (int i = 0; i < size; i++) {
           grid[i] = new int[size];
           for (int j = 0; j < size; j++) {
               grid[i][j] = -1;
           }
       }
   }


   // Parameterized constructor for custom board size
   GameState(int size) {
       this->size = size;
       currentTurn = 0;
       turnCount = 0;
       done = false;
       lastMove.set(-1, -1);
       grid = new int*[size];
       for (int i = 0; i < size; i++) {
           grid[i] = new int[size];
           for (int j = 0; j < size; j++) {
               grid[i][j] = -1;
           }
       }
   }


   // Copy constructor
   GameState(const GameState& other) {
       size = other.size;
       currentTurn = other.currentTurn;
       turnCount = other.turnCount;
       done = other.done;
       lastMove = other.lastMove;
       grid = new int*[size];
       for (int i = 0; i < size; i++) {
           grid[i] = new int[size];
           for (int j = 0; j < size; j++) {
               grid[i][j] = other.grid[i][j];
           }
       }
   }


   // Equality operator
   bool operator==(const GameState& other) {
       if (size != other.size || currentTurn != other.currentTurn ||
           turnCount != other.turnCount || done != other.done ||
           lastMove.x != other.lastMove.x || lastMove.y != other.lastMove.y) {
           return false;
       }
       for (int i = 0; i < size; i++) {
           for (int j = 0; j < size; j++) {
               if (grid[i][j] != other.grid[i][j]) {
                   return false;
               }
           }
       }
       return true;
   }


   // Assignment operator
   GameState& operator=(const GameState& other) {
       if (this != &other) {
           for (int i = 0; i < size; i++) {
               delete[] grid[i];
           }
           delete[] grid;


           size = other.size;
           currentTurn = other.currentTurn;
           turnCount = other.turnCount;
           done = other.done;
           lastMove = other.lastMove;
           grid = new int*[size];
           for (int i = 0; i < size; i++) {
               grid[i] = new int[size];
               for (int j = 0; j < size; j++) {
                   grid[i][j] = other.grid[i][j];
               }
           }
       }
       return *this;
   }


   // Check if a player has won
   bool hasWon(int player) {
       // Check rows
       for (int i = 0; i < size; i++) {
           bool win = true;
           for (int j = 0; j < size; j++) {
               if (grid[i][j] != player) {
                   win = false;
                   break;
               }
           }
           if (win) {
               return true;
           }
       }


       // Check columns
       for (int j = 0; j < size; j++) {
           bool win = true;
           for (int i = 0; i < size; i++) {
               if (grid[i][j] != player) {
                   win = false;
                   break;
               }
           }
           if (win) {
               return true;
           }
       }


       // Check main diagonal
       bool win = true;
       for (int i = 0; i < size; i++) {
           if (grid[i][i] != player) {
               win = false;
               break;
           }
       }
       if (win) {
           return true;
       }


       // Check secondary diagonal
       win = true;
       for (int i = 0; i < size; i++) {
           if (grid[i][size - 1 - i] != player) {
               win = false;
               break;
           }
       }
       if (win) {
           return true;
       }


       return false;
   }


   // Make a move on the board
   bool play(int x, int y) {
       if (x < 0 || x >= size || y < 0 || y >= size || grid[x][y] != -1) {
           return false;
       }
       grid[x][y] = currentTurn;
       currentTurn = !currentTurn;
       turnCount++;
       lastMove.set(x, y);
       if (hasWon(0) || hasWon(1) || turnCount == size * size) {
           done = true;
       }
       return true;
   }


   // Check if a move is valid
   bool isValidMove(int x, int y) {
       return x >= 0 && x < size && y >= 0 && y < size && grid[x][y] == -1;
   }


   // Check if the game is a draw
   bool isDraw() {
       return turnCount == size * size && !hasWon(0) && !hasWon(1);
   }


   // Get the current player (0 for X, 1 for O)
   int getCurrentPlayer() {
       return currentTurn;
   }


   // Destructor to free allocated memory
   ~GameState() {
       for (int i = 0; i < size; i++) {
           delete[] grid[i];
       }
       delete[] grid;
   }
};


// Overload << operator for GameState to allow easy printing of the game board
std::ostream& operator<<(std::ostream& os, const GameState& state) {
   os << "   ";
   for (int j = 0; j < state.size; j++) {
       os << " " << j << "  ";
   }
   os << std::endl;
   os << "   ";
   for (int j = 0; j < state.size; j++) {
       os << "--- ";
   }
   os << std::endl;
   for (int i = 0; i < state.size; i++) {
       os << i << " ";
       for (int j = 0; j < state.size; j++) {
           char c = ' ';
           if (state.grid[i][j] == 0) {
               c = 'X';
           } else if (state.grid[i][j] == 1) {
               c = 'O';
           }
           os << "| " << c << " ";
           if (j == state.size - 1) {
               os << "|";
           }
       }
       os << std::endl << "   ";
       for (int j = 0; j < state.size; j++) {
           os << "--- ";
       }
       os << std::endl;
   }
   return os;
}


#endif
