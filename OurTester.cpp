//
// Created by Owner on 25/04/2025.
//
#include <random>
#include "OurTester.h"
#include "iostream"
#include "GameBoard.h"
FILE* buildRandomBoardFile() {
    bool placed_tank1 = false;
    bool placed_tank2 = false;
    FILE *file = fopen("random_board.txt", "w");
    if (file == NULL) {
        std::cout << "Error opening file for writing.\n";
        return file;
    }
    std::uniform_int_distribution<> distrib(1,20); // Range: 1 to 20
    std::random_device rd;                         // Random number from hardware
    std::mt19937 gen(rd());
    int width = distrib(gen);
    int height = distrib(gen);
    fprintf(file, "%d %d\n", width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char c = '.';
            if (rand() % 5 == 0) {
                c = '#'; // Randomly place walls
            }
            else if (rand() % 10 == 0) {
                c = '@'; // Randomly place mines
            }
            else if (!placed_tank1 && rand() % 10 == 0) {
                c = '1'; // Randomly place tank 1
                placed_tank1 = true;
            } else if (!placed_tank2 && rand() % 10 == 0) {
                c = '2'; // Randomly place tank 2
                placed_tank2 = true;
            }
            fprintf(file, "%c", c);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return file;
}

bool testLoadingBoardFromFile() {
    FILE* file = buildRandomBoardFile();
    if (file == NULL) {
        std::cout << "Error opening file for reading.\n";
        return false;
    }
    GameBoard board;
    if (!board.loadBoardFromFile("random_board.txt")) {
        std::cout << "Failed to load board from file.\n";
        return false;
    }
    board.displayBoard();
    return true;
}