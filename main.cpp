#include <iostream>
#include <string>
#include "GameBoard.h"
#include "GameManager.h"

int main() {
    GameBoard gameBoard;
    std::string filename;
    std::cin >> filename;
    if (!gameBoard.loadBoardFromFile(filename)) {
        std::cerr << "Failed to load the game board from file." << std::endl;
        return 1;
    }
    GameManager game_manager(gameBoard);
    int step = 0;
    while(!game_manager.isGameOver() && game_manager.getMovesLeft() > 0) {
        if(step % 2 == 0) {
            game_manager.updateShells();
            game_manager.updateGame();
        }
        else {
            game_manager.updateShells();
        }
        gameBoard.displayBoard();
        step++;
        if(game_manager.getMovesLeft() <= 40) {
            game_manager.setMovesLeft(game_manager.getMovesLeft() - 1);
        }
    }
    return 0;
} 