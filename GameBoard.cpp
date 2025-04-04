
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
                tank1 = new Tank(x, count_lines, Direction::LEFT, '1',CanonDirection::L); // Assuming direction 0 for tank1
            } else if(c == '2') {
                board[count_lines][x] = CellType::TANK2;
                tank2 = new Tank(x, count_lines, Direction::RIGHT, '2', CanonDirection::R); // Assuming direction 1 for tank2
            } else if(c == '#') {
                board[count_lines][x] = CellType::WALL;
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
                case CellType::SHELL1:
                    std::cout << "o";
                    break;
                case CellType::SHELL2:
                    std::cout << "O";
                    break;
                case CellType::MINE:
                    std::cout << "@";
                    break;
            }
        }
        std::cout << std::endl;
    }
}
void GameBoard::placeTank(int x, int y, char index_tank, CanonDirection cdir) {
    if (index_tank == '1') {
        tank1 = new Tank(x, y, Direction::LEFT, index_tank, cdir);
        board[y][x] = CellType::TANK1;
    } else if (index_tank == '2') {
        tank2 = new Tank(x, y, Direction::RIGHT, index_tank, cdir);
        board[y][x] = CellType::TANK2;
    }
}
void GameBoard::placeShell(int x, int y) {
    //האם צריכה להיות הפרדה בין הפגז של טנק 1 לפגז של טנק 2?
        board[y][x] = CellType::SHELL1;
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
void GameBoard::shootFromTank(char index_tank, CanonDirection cdir) {
    if (index_tank == '1' && tank1) {
        // Logic to shoot from tank1
        switch(cdir) {
            case CanonDirection::U:
                placeShell(tank1->getX(), tank1->getY()+1);
                break;
            case CanonDirection::UR:
                placeShell(tank1->getX()+1, tank1->getY()+1);
                break;
            case CanonDirection::R:
                placeShell(tank1->getX()+1, tank1->getY());
                break;
            case CanonDirection::DR:
                placeShell(tank1->getX()+1, tank1->getY()-1);
                break;
            case CanonDirection::D:
                placeShell(tank1->getX(), tank1->getY()-1);
                break;
            case CanonDirection::DL:
                placeShell(tank1->getX()-1, tank1->getY()-1);
                break;
            case CanonDirection::L:
                placeShell(tank1->getX()-1, tank1->getY());
                break;
            case CanonDirection::UL:
                placeShell(tank1->getX()-1, tank1->getY()+1);
                break;
        }
        tank1->shoot();
    } else if (index_tank == '2' && tank2) {
        // Logic to shoot from tank2
        switch(cdir) {
            case CanonDirection::U:
                placeShell(tank2->getX(), tank2->getY()+1);
                break;
            case CanonDirection::UR:
                placeShell(tank2->getX()+1, tank2->getY()+1);
                break;
            case CanonDirection::R:
                placeShell(tank2->getX()+1, tank2->getY());
                break;
            case CanonDirection::DR:
                placeShell(tank2->getX()+1, tank2->getY()-1);
                break;
            case CanonDirection::D:
                placeShell(tank2->getX(), tank2->getY()-1);
                break;
            case CanonDirection::DL:
                placeShell(tank2->getX()-1, tank2->getY()-1);
                break;
            case CanonDirection::L:
                placeShell(tank2->getX()-1, tank2->getY());
                break;
            case CanonDirection::UL:
                placeShell(tank2->getX()-1, tank2->getY()+1);
                break;
        }
        tank2->shoot();
    }
}


