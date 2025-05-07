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

    //GameBoard game_board(width, height);
    if (!game_board.loadBoardFromFile(file_board, filename)) {
        file_errors << "Failed to load the game board from input." << std::endl;
        return 1;
    }

    //GameManager game_manager(std::make_shared<GameBoard>(game_board));
    GameManager game(MyPlayerFactory(), MyTankFactory());
    game.readBoard(file_board, filename);
    game.setMovesLeft(game.getBoard()->getMaxSteps());
    game.run();
    return 0;
}
