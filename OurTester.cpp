// OurTester.cpp
#include <random>
#include <fstream>
#include <iostream>
#include "OurTester.h"
#include "GameBoard.h"

std::string OurTester::buildRandomBoardFile() {
    // bool placed_player1 = false;
    // bool placed_player2 = false;
    std::string filename = "random_board.txt";
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file for writing.\n";
        return "";
    }

    std::cout << "Opening file for writing random board." << std::endl;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distrib(5, 10); // (Smaller boards are easier to debug)
    std::uniform_int_distribution<> cell_type(1, 100); 

    std::random_device rd;
 
    int width = distrib(gen);
    int height = distrib(gen);

    file << "Random Board\n";
    file<<"MaxSteps =" << cell_type(gen) << "\n";
    file << "NumShells =" << cell_type(gen) << "\n";
    file <<"Rows = " << height << "\n";
    file << "Cols =  " << width << "\n";

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char c = ' '; // Default empty cell
            int roll = cell_type(gen);
            if (roll <= 15) {
                c = '#'; // Wall
            }
            else if (roll <= 30) {
                c = '@'; // Mine
            }
            else if (roll <= 45) {
                c = '1'; // Tank 1
            }
            else if (roll <= 60) {
                c = '2'; // Tank 2
            }
            file << c;
        }
        file << "\n";
    }

    // Force place tanks if they were not placed randomly
    // if (!placed_tank1 || !placed_tank2) {
    //     std::cout << "Warning: One or both tanks not placed randomly. Please check file manually.\n";
    // }

    file.close();
    std::cout << "Random board file created successfully.\n";
    return filename;
}
