#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "GameBoard.h"
#include "GameManager.h"
#include "ActionType.h"

int main() {
    GameBoard game_board;
    std::string filename;

    std::cin >> filename;

    if (!game_board.loadBoardFromFile(filename)) {
        std::cerr << "Failed to load the game board from file." << std::endl;
        return 1;
    }
  
    GameManager game_manager(std::make_shared<GameBoard>(game_board));

    int step = 0;

    while (!game_manager.isGameOver() && game_manager.getMovesLeft() > 0) {
        std::cout << game_manager.getGameOver() << std::endl;

        // Step 1: Move all shells
        std::cout << "Moving shells." << std::endl;
        std::cout << "Shells size: " << game_manager.getmyShells().size() << std::endl;
        if (game_manager.getmyShells().empty()) {
            std::cout << "No shells to move." << std::endl;
        } else {
            game_manager.updateShells();

            // Step 2: Resolve shell collisions
            std::cout << "Resolving shell collisions." << std::endl;
            game_manager.resolveShellCollisions();
        }

        // Step 3: On even steps, update tanks and resolve tank collisions
        if (step % 2 == 0) {
            std::cout << "Updating tanks." << std::endl;
            game_manager.updateGame();
            std::cout << "Resolving tank collisions." << std::endl;
            game_manager.resolveTankCollisions();
        }

        // Step 4: Display the current game board
        game_board.displayBoard();

        // Step 5: Decrement moves left if in the final 40-move phase
        step++;
        if (game_manager.getMovesLeft() <= 40) {
            game_manager.setMovesLeft(game_manager.getMovesLeft() - 1);
        }

        std::cout << "Finished step " << step << std::endl;
    }

    std::cout << "Game Over: " << (game_manager.isGameOver() ? "Yes" : "No") << std::endl;

    // 🔵 [CHANGE] Added print before opening Output.txt
    std::cout << "Opening Output.txt for writing..." << std::endl;

    std::ofstream file("Output.txt");
    if (!file) {
        std::cerr << "Failed to open Output.txt for writing." << std::endl;
        return 1;
    }

    // Tank 1
    std::shared_ptr<Tank> tank1 = game_manager.getTank1();
    file << "Tank 1 Actions:\n";
    int count_actions1 = 1;
    int count_actions2 = 1;

    if (tank1) {
        for (const auto& action : tank1->getActions()) {
            file << count_actions1 << ". " << action << std::endl;
            count_actions1++;
        }
        file << "Reason of destruction: " << tank1->getDestructionCause() << std::endl;
    }

    // Tank 2
    std::shared_ptr<Tank> tank2 = game_manager.getTank2();
    file << "Tank 2 Actions:\n";
    if (tank2) {
        std::cout << "Finished step itai" << step << std::endl;
        for (const auto& action : tank2->getActions()) {
            file << count_actions2 << ". " << action << std::endl;
            count_actions2++; // 🔵 [FIX] Correct counter for Tank 2
        }
        file << "Reason of destruction: " << tank2->getDestructionCause() << std::endl;
    }

    file.close(); // 🔵 [NO CHANGE] - good, closing properly

    // 🔵 [CHANGE] Added print after writing
    std::cout << "Finished writing to Output.txt successfully." << std::endl;

    std::cout << "Game Over: " << (game_manager.isGameOver() ? "Yes" : "No") << std::endl;
    return 0;
}
