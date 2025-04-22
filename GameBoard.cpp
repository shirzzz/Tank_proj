#include "GameBoard.h"
#include "CellType.h"
#include "Tank.h"
#include "Shell.h"
#include "CanonDirection.h"

#include <iostream>
#include <fstream>
#include <vector>

bool GameBoard::loadBoardFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    file >> width >> height;
    file.ignore(); // Ignore the newline after width and height

    board.clear();
    walls.resize(height, std::vector<Wall>(width));
    board.resize(height, std::vector<CellType>(width, CellType::EMPTY));

    std::string line;
    int count_lines = 0;

    while (std::getline(file, line)) {
        if (line.empty()) {
            std::cerr << "Error: Empty line in file." << std::endl;
            return false;
        }

        if (count_lines >= height) break;

        if (line.length() < static_cast<size_t>(width)) {
            std::cerr << "Error: Line length is less than expected width." << std::endl;
            return false;
        }

        for (int x = 0; x < width; ++x) {
            char c = line[x];
            switch (c) {
                case '1':
                    board[count_lines][x] = CellType::TANK1;
                    tank1 = new Tank(x, count_lines, CanonDirection::L, '1');
                    break;
                case '2':
                    board[count_lines][x] = CellType::TANK2;
                    tank2 = new Tank(x, count_lines, CanonDirection::R, '2');
                    break;
                case '#':
                    board[count_lines][x] = CellType::WALL;
                    walls[count_lines][x] = Wall(x, count_lines);
                    break;
                case '@':
                    board[count_lines][x] = CellType::MINE;
                    break;
                case ' ':
                    board[count_lines][x] = CellType::EMPTY;
                    break;
                default:
                    std::cerr << "Error: Invalid character in file: " << c << std::endl;
                    return false;
            }
        }
        ++count_lines;
    }

    if (count_lines < height) {
        std::cerr << "Error: Not enough lines in file." << std::endl;
        return false;
    }

    file.close();
    return true;
}

void GameBoard::displayBoard() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            switch (board[y][x]) {
                case CellType::EMPTY:  std::cout << " "; break;
                case CellType::WALL:   std::cout << "#"; break;
                case CellType::TANK1:  std::cout << "1"; break;
                case CellType::TANK2:  std::cout << "2"; break;
                case CellType::SHELL:  std::cout << "o"; break;
                case CellType::MINE:   std::cout << "@"; break;
            }
        }
        std::cout << std::endl;
    }
}

void GameBoard::placeShell(int x, int y) {
    // Future enhancement: differentiate shells by tank
    board[y][x] = CellType::SHELL;
}

void GameBoard::moveTank(char tankIndex, int newX, int newY) {
    if (tankIndex == '1' && tank1) {
        board[tank1->getY()][tank1->getX()] = CellType::EMPTY;
        tank1->setX(newX);
        tank1->setY(newY);
        board[newY][newX] = CellType::TANK1;
    } else if (tankIndex == '2' && tank2) {
        board[tank2->getY()][tank2->getX()] = CellType::EMPTY;
        tank2->setX(newX);
        tank2->setY(newY);
        board[newY][newX] = CellType::TANK2;
    }
}

bool GameBoard::isCellWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    CellType c = board[y][x];
    return c != CellType::MINE && c != CellType::WALL;
}

CellType GameBoard::getCell(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return CellType::WALL; // Out-of-bounds is treated as a wall
    }
    return board[y][x];
}

int GameBoard::getWidth() const { return width; }
int GameBoard::getHeight() const { return height; }
