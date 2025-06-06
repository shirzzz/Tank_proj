#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "GameBoard.h"
#include "GameManager.h"
//#include "ActionRequest.h"
#include "MyPlayerFactory.h"
#include "MyTankAlgorithmFactory.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <game_board_input_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    // Assignment 2 API - Create factories
    MyPlayerFactory playerFactory;
    MyTankAlgorithmFactory tankFactory;
    
    // Assignment 2 API - Create GameManager with factories
    GameManager game(playerFactory, tankFactory);
    
    // Read board and run
    game.readBoard(filename);
    game.run();
    
    return 0;
}