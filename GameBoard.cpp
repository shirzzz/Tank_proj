#include "GameBoard.h"
#include "CellType.h"
#include "Tank.h"
#include "Shell.h"
#include "CanonDirection.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "BFSChaserAI.h"
#include "Chased.h"
bool GameBoard::loadBoardFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    file >> width >> height;
    file.ignore(); // Ignore the newline after width and height

    board.clear();
    walls.resize(height, std::vector<Wall>(width, Wall(0, 0)));
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

void GameBoard::moveShell(Shell *shell) {
    int dx = 0, dy = 0;
    CanonDirection direction = shell->getDirection();
    board[shell->getY()][ shell->getX()] = CellType::EMPTY; // Clear previous position
    int speed = 1; // Speed of the shell
    switch (direction) {
        case CanonDirection::U:  dy = speed; break;
        case CanonDirection::UR: dx = speed; dy = speed; break;
        case CanonDirection::R:  dx = speed; break;
        case CanonDirection::DR: dx = speed; dy = -speed; break;
        case CanonDirection::D:  dy = -speed; break;
        case CanonDirection::DL: dx = -speed; dy = -speed; break;
        case CanonDirection::L:  dx = -speed; break;
        case CanonDirection::UL: dx = -speed; dy = speed; break;
    }

    int x_moved = shell->getX() + dx;
    int y_moved = shell->getY() + dy;

    // Wrap around horizontally
    if (x_moved < 0) x_moved += width;
    if (x_moved >= width) x_moved -= width;

    // Wrap around vertically
    if (y_moved < 0) y_moved += height;
    if (y_moved >= height) y_moved -= height;

    // Update position
    shell->setX(x_moved);
    shell->setY(y_moved);

    // Update the shell's position on the game board
    updateShellPosition(shell, shell->getX(), shell->getY());

    // Check for collision with walls or tanks
    if (!isCellWalkable(shell->getX(), shell->getY())) {
        // Handle collision (e.g., remove shell)
    }
    board[shell->getY()][shell->getX()] = CellType::SHELL; // Update the board with the shell's new position


}
ActionType GameBoard::movingAlgorithm(Tank& tank) {
    if (tank.getIndexTank() == '1') {
        BFSChaserAI ai;
        return ai.decideNextAction(*this, *tank1, *tank2);
    } else {
        Chased chasedAI;
        return chasedAI.decideNextAction(*this, *tank2, *tank1);
    }
}
int GameBoard::getWidth() const { return width; }
int GameBoard::getHeight() const { return height; }
