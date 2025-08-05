#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "OurTester.h"
#include "Algorithm/GameBoard.h"
#include "GameManager/GameManager.h"
//#include "ActionRequest.h"
#include "UserCommon/MyPlayerFactory.h"
#include "UserCommon/MyTankAlgorithmFactory.h"
//#include "random"
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <game_board_input_file>" << std::endl;
        return 1;
    }
        
    std::cout << "Starting the game with input file: " << argv[1] << std::endl;
    std::string filename = argv[1]; //Originally used argv[1] directly, but we will use a fixed filename for testing
    // OurTester tester;
    // std::random_device rd;  // Seed (may be slow but good)
    // std::mt19937 gen(rd()); // Mersenne Twister engine
    // std::uniform_int_distribution<> dist(1, 10);  // range: [1, 100]
    // int i = dist(gen);
    // std::string filename = tester.buildRandomBoardFile(i);
    std::cout << "Reading game board from file: " << filename << std::endl;
    // Assignment 2 API - Create factories
    MyPlayerFactory playerFactory;
    MyTankAlgorithmFactory tankFactory;
    std::cout << "Creating player and tank algorithm factories." << std::endl;
    // Assignment 2 API - Create GameManager with factories
    GameManager_211466123_212399455::GameManager game(playerFactory, tankFactory);
    std::cout << "GameManager created with player and tank algorithm factories." << std::endl;
    game.setOutputFileNameFromInput(filename);
    // Read board and run
    game.readBoard(filename);
    std::cout << "Board read successfully. Starting the game." << std::endl;
    game.run();
    std::cout << "Game finished." << std::endl;
    return 0;
}