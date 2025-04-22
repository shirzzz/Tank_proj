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
    while(!game_manager.isGameOver()) {
        if(step % 2 == 0) {
            game_manager.updateShells();
            game_manager.updateGame();
        }
        else {
            game_manager.updateShells();
        }
        gameBoard.displayBoard();
        step++;
    }
    return 0;
} 