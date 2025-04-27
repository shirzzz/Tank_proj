// OurTester.cpp
#include <random>
#include <fstream>
#include <iostream>
#include "OurTester.h"
#include "GameBoard.h"

std::string OurTester::buildRandomBoardFile() {
    bool placed_tank1 = false;
    bool placed_tank2 = false;
    std::string filename = "random_board.txt";
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file for writing.\n";
        return "";
    }

    std::cout << "Opening file for writing random board." << std::endl;

    std::uniform_int_distribution<> distrib(5, 10); // (Smaller boards are easier to debug)
    std::random_device rd;
    std::mt19937 gen(rd());
    int width = distrib(gen);
    int height = distrib(gen);
    file << width << " " << height << "\n";

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char c = '.';
            if (rand() % 5 == 0) {
                c = '#'; // Wall
            }
            else if (rand() % 10 == 0) {
                c = '@'; // Mine
            }
            else if (!placed_tank1 && rand() % 10 == 0) {
                c = '1'; // Tank 1
                placed_tank1 = true;
            }
            else if (!placed_tank2 && rand() % 10 == 0) {
                c = '2'; // Tank 2
                placed_tank2 = true;
            }
            file << c;
        }
        file << "\n";
    }

    // Force place tanks if they were not placed randomly
    if (!placed_tank1 || !placed_tank2) {
        std::cout << "Warning: One or both tanks not placed randomly. Please check file manually.\n";
    }

    file.close();
    std::cout << "Random board file created successfully.\n";
    return filename;
}
