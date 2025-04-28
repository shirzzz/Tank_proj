#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "GameBoard.h"
#include "GameManager.h"
#include "ActionType.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <game_board_input_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file_board(filename);
    std::ofstream file_errors("input_errors.txt");

    if (!file_board.is_open()) {
        file_errors << "Error opening file of the board: " << filename << std::endl;
        file_errors.close();
        std::cerr << "Error opening file of the board: " << filename << std::endl;
        return 1;
    }

    int width, height;
    if (!(file_board >> width >> height)) {
        file_errors << "Error reading width and height from input." << std::endl;
        return 1;
    }

    GameBoard game_board(width, height);
    if (!game_board.loadBoardFromFile(file_board, filename)) {
        file_errors << "Failed to load the game board from input." << std::endl;
        return 1;
    }

    GameManager game_manager(std::make_shared<GameBoard>(game_board));
    int step = 0;
    while (!game_manager.isGameOver() && game_manager.getMovesLeft() > 0) {
        game_manager.updateShells();
        game_manager.resolveShellCollisions();

        if (step % 2 == 0) {
            game_manager.updateGame();
            game_manager.resolveTankCollisions();
        }

        game_board.displayBoard();
        step++;
        if (game_manager.getMovesLeft() <= 40) {
            game_manager.setMovesLeft(game_manager.getMovesLeft() - 1);
            if (game_manager.getMovesLeft() == 0) {
                game_manager.endGame();
                std::cout << "Game Over: No moves left!" << std::endl;
                game_manager.getTank1()->setDestructionCause(DestructionCause::OUTOFAMMO);
                game_manager.getTank2()->setDestructionCause(DestructionCause::OUTOFAMMO);
            }
        }
    }

    std::ofstream file("Output_" + filename + ".txt");
    if (!file) {
        std::cerr << "Failed to open Output file for writing." << std::endl;
        return 1;
    }

    file << "And the winner is: ";
    if (game_manager.getTank1()->getActions().back() == ActionType::WIN) {
        file << "Player 1 :) " << std::endl;
    } else if (game_manager.getTank2()->getActions().back() == ActionType::WIN) {
        file << "Player 2 :) " << std::endl;
    } else {
        file << "they both won because it is a draw!!" << std::endl;
    }

    // Tank 1
    std::shared_ptr<Tank> tank1 = game_manager.getTank1();
    file << "Tank 1 Actions:\n";
    int count_actions1 = 1;
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
    int count_actions2 = 1;
    if (tank2) {
        for (const auto& action : tank2->getActions()) {
            file << count_actions2 << ". " << action << std::endl;
            count_actions2++;
        }
        file << "Reason of destruction: " << tank2->getDestructionCause() << std::endl;
    }

    file.close();
    std::cout << "Finished writing to Output_" << filename << ".txt successfully." << std::endl;
    return 0;
}
