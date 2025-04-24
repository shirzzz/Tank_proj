#include "GameBoard.h"
#include "CellType.h"
#include "Tank.h"
#include "Shell.h"
#include "CanonDirection.h"
#include "Shape.h"
#include "Empty.h"
#include "Wall.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "BFSChaserAI.h"
#include "Chased.h"
#include "Mine.h"

bool GameBoard::loadBoardFromFile(const std::string &filename) {
    int count_tanks_for_player1 = 0;
    int count_tanks_for_player2 = 0;
    std::ofstream file_errors("input_errors.txt");
    if (!file_errors) {
        std::cerr << "Failed to open file for writing input errors." << std::endl;
        return false;
    }
    std::ifstream file_board(filename);
    if (!file_board.is_open()) {
        std::cerr << "Error opening file of the board: " << filename << std::endl;
        return false;
    }

    file_board >> width >> height;
    file_board.ignore(); // Ignore the newline after width and height

    board = std::make_shared<std::vector<std::vector<Shape*>>>(height, std::vector<Shape*>(width, new Empty(-1, -1)));

    std::string line;
    int count_lines = 0;

    while (std::getline(file_board, line)) {
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
                    if (count_tanks_for_player1 >= 1) {
                        std::cerr << "Error: More than one tank for player 1." << std::endl;
                        return false;
                    }
                    tank1 = std::make_shared<Tank>(x, count_lines, '1');
                    (*board)[count_lines][x] = tank1.get();
                    count_tanks_for_player1++;
                    break;
                case '2':
                    if (count_tanks_for_player2 >= 1) {
                        std::cerr << "Error: More than one tank for player 2." << std::endl;
                        return false;
                    }
                    tank2 = std::make_shared<Tank>(x, count_lines, '2');
                    (*board)[count_lines][x] = tank2.get();
                    count_tanks_for_player2++;
                    break;
                case '#':
                    (*board)[count_lines][x] = new Wall(x, count_lines);
                    break;
                case '@':
                    (*board)[count_lines][x] = new Mine(x, count_lines);
                    break;
                case ' ':
                    (*board)[count_lines][x] = new Empty(x, count_lines);
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

    file_board.close();
    return true;
}

void  GameBoard::displayBoard() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((*board)[y][x]) {
                switch ((*board)[y][x]->getCellType()) {
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
                    default:
                        std::cout << "?"; // Handle unknown cell types
                        break;
                }
            } else {
                std::cout << "?"; // Handle null pointer case
            }
        }
        std::cout << std::endl;
    }
}

// void GameBoard::placeShell(int x, int y) const {
//     (*board)[y][x] = new Shell(x, y, CanonDirection::U); // Default direction, might need to be updated
// }
void GameBoard::updateShellPosition(Shell *shell, int newX, int newY) {
    if (shell) {
        shell->setX(newX);
        shell->setY(newY);
    }
}

void GameBoard::moveTank(char tankIndex, int newX, int newY) const {
    if (tankIndex == '1' && tank1) {
        (*board)[tank1->getY()][tank1->getX()] = new Empty(tank1->getX(), tank1->getY());
        tank1->setX(newX);
        tank1->setY(newY);
        (*board)[newY][newX] = tank1.get();
    } else if (tankIndex == '2' && tank2) {
        (*board)[tank2->getY()][tank2->getX()] = new Empty(tank2->getX(), tank2->getY());
        tank2->setX(newX);
        tank2->setY(newY);
        (*board)[newY][newX] = tank2.get();
    }
}

bool GameBoard::isCellWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if ((*board)[y][x]) {
        CellType c = (*board)[y][x]->getCellType();
        return c != CellType::MINE && c != CellType::WALL && c != CellType::TANK1 && c != CellType::TANK2 && c != CellType::SHELL;
    }
    return false; // Consider null pointers as not walkable
}


void GameBoard::moveShell(Shell *shell) {
    int dx = 0, dy = 0;
    CanonDirection direction = shell->getDirection();
    (*board)[shell->getY()][shell->getX()] = new Empty(shell->getX(), shell->getY()); // Clear previous position
    int speed = 1; // Speed of the shell
    switch (direction) {
        case CanonDirection::U:
            dy = -speed;
            break;
        case CanonDirection::UR:
            dx = speed;
            dy = -speed;
            break;
        case CanonDirection::R:
            dx = speed;
            break;
        case CanonDirection::DR:
            dx = speed;
            dy = speed;
            break;
        case CanonDirection::D:
            dy = speed;
            break;
        case CanonDirection::DL:
            dx = -speed;
            dy = speed;
            break;
        case CanonDirection::L:
            dx = -speed;
            break;
        case CanonDirection::UL:
            dx = -speed;
            dy = -speed;
            break;
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
        (*board)[shell->getY()][shell->getX()] = new Empty(shell->getX(), shell->getY()); // Remove shell from board
        delete shell; // Important to deallocate memory if shell is destroyed
        shell = nullptr; // Set pointer to null to avoid dangling pointer
        return;
    }
    (*board)[shell->getY()][shell->getX()] = new Shell(shell->getX(), shell->getY(), shell->getDirection()); // Update the board with the shell's new position
}



ActionType GameBoard::movingAlgorithm(Tank &tank) {
    if (tank.getIndexTank() == '1' && tank1) {
        BFSChaserAI ai;
        return ai.decideNextAction(*this, *tank1, *tank2);
    } else if (tank.getIndexTank() == '2' && tank2) {
        Chased chasedAI;
        return chasedAI.decideNextAction(*this, *tank2, *tank1);
    }
    return ActionType::INVALID_ACTION; // Default action if tank is not found
}

int GameBoard::getWidth() const {
    return width;
}

int GameBoard::getHeight() const {
    return height;
}