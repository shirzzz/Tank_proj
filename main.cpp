#include <iostream>
#include <string>
#include "GameBoard.h"
int main() {
    GameBoard gameBoard;
    std::string filename;
    std::cin >> filename;
    if (!gameBoard.loadBoardFromFile(filename)) {
        std::cerr << "Failed to load the game board from file." << std::endl;
        return 1;
    }
    
    return 0;
} 