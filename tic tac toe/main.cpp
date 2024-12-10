#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "GameState.h"


using namespace std;


// Function to get a move from a human player
Vec getHumanMove(GameState game) {
   int x, y;
   string input;
   bool validInput;
   do {
       validInput = true;
       cout << "Enter row (0-" << game.size - 1 << "): ";
       cin >> input;
       try {
           x = stoi(input);
       } catch (const invalid_argument& e) {
           cout << "Invalid input. Please enter a number." << endl;
           validInput = false;
           cin.clear();
           cin.ignore(numeric_limits<streamsize>::max(), '\n');
           continue;
       }
       
       cout << "Enter column (0-" << game.size - 1 << "): ";
       cin >> input;
       try {
           y = stoi(input);
       } catch (const invalid_argument& e) {
           cout << "Invalid input. Please enter a number." << endl;
           validInput = false;
           cin.clear();
           cin.ignore(numeric_limits<streamsize>::max(), '\n');
           continue;
       }
       
       if (!game.isValidMove(x, y)) {
           cout << "Invalid move. Please try again." << endl;
           validInput = false;
       }
   } while (!validInput);
   return Vec(x, y);
}


// Function to generate a random valid move for the AI
Vec validMove(GameState game) {
   int x, y;
   do {
       x = rand() % game.size;
       y = rand() % game.size;
   } while (!game.isValidMove(x, y));
   return Vec(x, y);
}


// Minimax algorithm implementation with alpha-beta pruning
int minimax(GameState game, int depth, bool isMaximizingPlayer, int alpha, int beta) {
   if (game.hasWon(0)) {
       return -1;
   } else if (game.hasWon(1)) {
       return 1;
   } else if (game.isDraw()) {
       return 0;
   }


   if (isMaximizingPlayer) {
       int maxEval = -1000000;
       for (int i = 0; i < game.size; i++) {
           for (int j = 0; j < game.size; j++) {
               if (game.isValidMove(i, j)) {
                   GameState newGame = game;
                   newGame.play(i, j);
                   int eval = minimax(newGame, depth + 1, false, alpha, beta);
                   maxEval = max(maxEval, eval);
                   alpha = max(alpha, eval);
                   if (beta <= alpha) {
                       break;
                   }
               }
           }
       }
       return maxEval;
   } else {
       int minEval = 1000000;
       for (int i = 0; i < game.size; i++) {
           for (int j = 0; j < game.size; j++) {
               if (game.isValidMove(i, j)) {
                   GameState newGame = game;
                   newGame.play(i, j);
                   int eval = minimax(newGame, depth + 1, true, alpha, beta);
                   minEval = min(minEval, eval);
                   beta = min(beta, eval);
                   if (beta <= alpha) {
                       break;
                   }
               }
           }
       }
       return minEval;
   }
}


// Function to find the best move for the AI using minimax
Vec findBestMove(GameState game) {
   int bestScore = -1000000;
   Vec bestMove(-1, -1);
   for (int i = 0; i < game.size; i++) {
       for (int j = 0; j < game.size; j++) {
           if (game.isValidMove(i, j)) {
               GameState newGame = game;
               newGame.play(i, j);
               int score = minimax(newGame, 0, false, -1000000, 1000000);
               if (score > bestScore) {
                   bestScore = score;
                   bestMove = Vec(i, j);
               }
           }
       }
   }
   return bestMove;
}


// Main game loop function
void playGame(GameState game, bool humanOpponent1, bool humanOpponent2, bool sophisticatedComputer1, bool sophisticatedComputer2) {
   while (!game.done) {
       cout << game << endl;


       Vec move;
       if (game.getCurrentPlayer() == 0) {
           if (humanOpponent1) {
               cout << "Player X's turn." << endl;
               move = getHumanMove(game);
           } else if (sophisticatedComputer1) {
               move = findBestMove(game);
           } else {
               move = validMove(game);
           }
       } else {
           if (humanOpponent2) {
               cout << "Player O's turn." << endl;
               move = getHumanMove(game);
           } else if (sophisticatedComputer2) {
               move = findBestMove(game);
           } else {
               move = validMove(game);
           }
       }


       game.play(move.x, move.y);
   }


   cout << game << endl;
   if (game.hasWon(0)) {
       if (humanOpponent1 && humanOpponent2) {
           cout << "Player 1 wins!" << endl;
       } else if (sophisticatedComputer1) {
           cout << "AI wins!" << endl;
       } else if (!humanOpponent1 && !sophisticatedComputer1) {
           cout << "AI wins!" << endl;
       } else {
           cout << "\033[1;32mYou Win!\033[0m" << endl;
       }
   } else if (game.hasWon(1)) {
       if (humanOpponent1 && humanOpponent2) {
           cout << "Player 2 wins!" << endl;
       } else if (sophisticatedComputer2) {
           cout << "AI wins!" << endl;
       } else if (!humanOpponent2 && !sophisticatedComputer2) {
           cout << "AI wins!" << endl;
       } else {
           cout << "Player O wins!" << endl;
       }
   } else {
       cout << "It's a draw!" << endl;
   }
}


// Function to display the main menu
void displayMenu() {
    cout << "\n\033[1;36m╔════════════════════════╗\033[0m" << endl;
    cout << "\033[1;36m║     \033[1;33mTic-Tac-Toe\033[1;36m      ║\033[0m" << endl;
    cout << "\033[1;36m╠════════════════════════╣\033[0m" << endl;
    cout << "\033[1;36m║ \033[1;32m1. Player vs. Player   \033[1;36m║\033[0m" << endl;
    cout << "\033[1;36m║ \033[1;32m2. Player vs. Easy AI  \033[1;36m║\033[0m" << endl;
    cout << "\033[1;36m║ \033[1;32m3. Player vs. Hard AI  \033[1;36m║\033[0m" << endl;
    cout << "\033[1;36m║ \033[1;32m4. Quit               \033[1;36m║\033[0m" << endl;
    cout << "\033[1;36m╚════════════════════════╝\033[0m" << endl;
    cout << "\033[1;35mEnter your choice: \033[0m";
}


// Main function
int main() {
   srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

   int choice;
   string input;
   do {
       displayMenu(); // Show the main menu
       cin >> input;

       try {
           choice = stoi(input);

           switch (choice) {
               case 1:
                   // Player vs. Player
                   playGame(GameState(), true, true, false, false);
                   break;
               case 2:
                   // Player vs. Easy AI
                   playGame(GameState(), true, false, false, false);
                   break;
               case 3:
                   // Player vs. Hard AI
                   playGame(GameState(), true, false, false, true);
                   break;
               case 4:
                   // Quit the game
                   cout << "\033[1;32mThanks for playing!\033[0m" << endl;
                   return 0;
               default:
                   cout << "\033[1;31mInvalid choice. Please try again.\033[0m" << endl;
           }
       } catch (const invalid_argument& e) {
           cout << "\033[1;31mInvalid input. Please enter a number.\033[0m" << endl;
       }

       // Clear input buffer
       cin.clear();
       cin.ignore(numeric_limits<streamsize>::max(), '\n');
   } while (true); // Loop continues until the user chooses to quit

   return 0;
}
