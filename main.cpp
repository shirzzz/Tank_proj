#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "GameBoard.h"
#include "GameManager.h"
#include "ActionType.h"

int main() {
    GameBoard gameBoard;
    std::string filename;

    std::cin >> filename;

    if (!gameBoard.loadBoardFromFile(filename)) {
        std::cerr << "Failed to load the game board from file." << std::endl;
        return 1;
    }
  
    GameManager game_manager(gameBoard.getGameBoard());
   
    int step = 0;

    while (!game_manager.isGameOver() && game_manager.getMovesLeft() > 0) {
        // Step 1: Move all shells
        std::cout<<"Moving shells."<<std::endl;
        game_manager.updateShells();

        // Step 2: Resolve shell collisions
        std::cout<<"Resolving shell collisions."<<std::endl;
        game_manager.resolveShellCollisions();

        // Step 3: On even steps, update tanks and resolve tank collisions
        if (step % 2 == 0) {
            std::cout<<"Updating tanks."<<std::endl;
            game_manager.updateGame();
            std::cout<<"Resolving tank collisions."<<std::endl;
            game_manager.resolveTankCollisions();
        }

        // Step 4: Display the current game board
        

        // Step 5: Decrement moves left if in the final 40-move phase
        step++;
        if (game_manager.getMovesLeft() <= 40) {
            game_manager.setMovesLeft(game_manager.getMovesLeft() - 1);
        }
        gameBoard.displayBoard();
    }

    // Step 6: Write actions and termination reasons to a file
    std::ofstream file("Output.txt");
    if (!file) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    // Tank 1
    //
    std::shared_ptr<Tank> tank1 = game_manager.getTank1();
    file << "Tank 1 Actions:\n";
    if (tank1) {
        for (const auto& action : tank1->getActions()) {
            file << "reason: " << action << std::endl;
        }
        file << tank1->getReason() << std::endl;
    }

    // Tank 2
    std::shared_ptr<Tank> tank2 = game_manager.getTank2();
    file << "Tank 2 Actions:\n";
    if (tank2) {
        for (const auto& action : tank2->getActions()) {
            file << action << std::endl;
        }
        file << "reason: " << tank2->getReason() << std::endl;
    }

    file.close();
    return 0;
}
