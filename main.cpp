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
    while(!game_manager.isGameOver()) {
        game_manager.updateGame();
        game_manager.updateShells();
        gameBoard.displayBoard();
    }
    return 0;
} 