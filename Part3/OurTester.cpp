// OurTester.cpp
#include <random>
#include <fstream>
#include <iostream>
#include "OurTester.h"
#include "Algorithm/GameBoard.h"

std::string OurTester::buildRandomBoardFile(int i) {
    // bool placed_player1 = false;
    // bool placed_player2 = false;
    std::string filename = "My_amazing_map_"+std::to_string(i)+".txt";
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
            else if (roll >= 60 && roll <= 70) {
                // Randomly place tanks with a 10% chance
                if (j % 2 == 0) { // Alternate placement for variety
                    c = '1'; // Tank 1
                    // placed_tank1 = true;
                } else {
                    c = '2'; // Tank 2
                    // placed_tank2 = true;
                }
            }
            file << c;
        }
        file << "\n";
    }

    file.close();
    std::cout << "Random board file created successfully.\n";
    return filename;
}
