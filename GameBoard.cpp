
#include "GameBoard.h"
#include "CellType.h"
#include "Tank.h"
#include "Shell.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "CanonDirection.h"

bool GameBoard::loadBoardFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }
    file >> width >> height;
    file.ignore(); // Ignore the newline after width and height
    board.clear();
    walls.resize(height);
    for (int i = 0; i < height; ++i) {
        walls[i].resize(width);
    }
    std::string line;
    int count_lines = 0;
    board.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    while(std::getline(file, line)) {
        // Check if the line is empty
        if(line.empty()) {
            std::cerr << "Error: Empty line in file." << std::endl;
            return false;
        }
        // Check if there is not enough lines in the file
        if(count_lines >= height) {
            break;
        }
        // Check if the line is shorter than expected
        if(line.length() < width) {
            std::cerr << "Error: Line length is less than expected width." << std::endl;
            return false;
        }
        for(int x = 0; x<width; x++) {
            char c = line[x];
            if(c == '1') {
                board[count_lines][x] = CellType::TANK1;
                tank1 = new Tank(x, count_lines, CanonDirection::L, '1'); // Assuming direction 0 for tank1
            } else if(c == '2') {
                board[count_lines][x] = CellType::TANK2;
                tank2 = new Tank(x, count_lines, CanonDirection::R, '2'); // Assuming direction 1 for tank2
            } else if(c == '#') {
                board[count_lines][x] = CellType::WALL;
                walls[count_lines][x] = Wall(x, count_lines); // Assuming Wall constructor takes x and y
            } else if(c == '@'){
                board[count_lines][x] = CellType::MINE;
            }
            else if(c == ' ') {
                board[count_lines][x] = CellType::EMPTY;
            } else {
                std::cerr << "Error: Invalid character in file: " << c << std::endl;
                return false;
            }
        }
        count_lines++;
    }
    if(count_lines < height) {
        std::cerr << "Error: Not enough lines in file." << std::endl;
        return false;
    }
    file.close();
    return true;
}
void GameBoard::displayBoard() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (board[y][x]) {
                case CellType::EMPTY:
                    std::cout << " ";
                    break;
                case CellType::WALL:
                    std::cout << "#";
                    break;
                case CellType::TANK1:
                    std::cout << "1";
                    break;
                case CellType::TANK2:
                    std::cout << "2";
                    break;
                case CellType::SHELL:
                    std::cout << "o";
                    break;
                case CellType::MINE:
                    std::cout << "@";
                    break;
            }
        }
        std::cout << std::endl;
    }
}
void GameBoard::placeShell(int x, int y) {
    //האם צריכה להיות הפרדה בין הפגז של טנק 1 לפגז של טנק 2?
        board[y][x] = CellType::SHELL;
}
void GameBoard::moveTank(char tankIndex, int newX, int newY) {
    if (tankIndex == '1' && tank1) {
        board[tank1->getY()][tank1->getX()] = CellType::EMPTY; // Clear old position
        tank1->setX(newX);
        tank1->setY(newY);
        board[newY][newX] = CellType::TANK1; // Set new position
    } else if (tankIndex == '2' && tank2) {
        board[tank2->getY()][tank2->getX()] = CellType::EMPTY; // Clear old position
        tank2->setX(newX);
        tank2->setY(newY);
        board[newY][newX] = CellType::TANK2; // Set new position
    }
}

    
    bool GameBoard::isCellWalkable(int x, int y) const {
        if (x < 0 || y < 0 || y >= height || x >= width) return false;
        CellType c = board[y][x];
        return c != CellType::MINE && c != CellType::WALL;  // walls and mines are not walkable
    }

    CellType GameBoard::getCell(int x, int y) const {
        if (x < 0 || y < 0 || y >= height || x >= width) {
            return CellType::WALL; // Treat out-of-bounds as wall
        }
        return board[y][x];
    }
    
    int GameBoard::getWidth() const { return width; }
    int GameBoard::getHeight() const { return height; }
}


