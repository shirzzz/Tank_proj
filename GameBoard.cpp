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
#include "Chased.h"
#include "Mine.h"
#include "BfsChaserShir.h"

bool GameBoard::loadBoardFromFile(std::ifstream& file_board, std::string filename) {
    // std::cout << "Loading board from file: " << filename << std::endl;
    int count_tanks_for_player1 = 0;
    //int count_walls = 0;
    int count_tanks_for_player2 = 0;
    std::ofstream file_errors("input_errors.txt");
    std::cout << "Opening file for writing input errors." << std::endl;
    if (!file_errors) {
        file_errors << "Error: Failed to open file for writing input errors.\n" << std::endl;
        file_errors.close();
        std::cerr << "Failed to open file for writing input errors." << std::endl;
        return false;
    }
    file_board.ignore(); // Ignore the newline after width and height
    if (width <= 0 || height <= 0) {
        file_errors << "Error: Invalid board dimensions." << std::endl;
        file_errors.close();
        std::cerr << "Error: Invalid board dimensions." << std::endl;
        return false;
    }
    std::string line;
    int count_lines = 0;

    while (std::getline(file_board, line)) {
        if (line.empty()) {
            file_errors << "Error: Empty line in file." << filename<<std::endl;
            file_errors.close();
            std::cerr << "Error: Empty line in file." << std::endl;
            return false;
        }

        if (count_lines >= height) break;

        if (line.length() < static_cast<size_t>(width)) {
            file_errors << "Error: Line length is less than expected width." << std::endl;
            file_errors.close();
            std::cerr << "Error: Line length is less than expected width." << std::endl;
            return false;
        }

        for (int x = 0; x < width; ++x) {
            char c = line[x];
            switch (c) {
                case '1':
                    if (count_tanks_for_player1 >= 1) {
                        file_errors << "Error: More than one tank for player 1." << std::endl;
                        file_errors.close();
                        std::cerr << "Error: More than one tank for player 1." << std::endl;
                        return false;
                    }
                    tank1 = std::make_shared<Tank>(x, count_lines, '1');
                    (*board)[count_lines][x] = tank1.get();
                    count_tanks_for_player1++;
                    std::cout<<"Tank1 position "<<"("<< tank1.get()->getX()<<","<<tank1.get()->getY()<<")"<<std::endl;
                    break;
                case '2':
                    
                    if (count_tanks_for_player2 >= 1) {
                        file_errors << "Error: More than one tank for player 2." << std::endl;
                        file_errors.close();
                        std::cerr << "Error: More than one tank for player 2." << std::endl;
                        return false;
                    }
                    tank2 = std::make_shared<Tank>(x, count_lines, '2');
                    (*board)[count_lines][x] = tank2.get();
                    count_tanks_for_player2++;
                    std::cout<<"Tank2 position "<<"("<< tank2.get()->getX()<<","<<tank2.get()->getY()<<")"<<std::endl;
                    break;
                case '#':
                    (*board)[count_lines][x] = new Wall(x, count_lines);
                    num_walls++;
                    break;
                case '@':
                    (*board)[count_lines][x] = new Mine(x, count_lines);
                    break;
                case ' ':
                    (*board)[count_lines][x] = new Empty(x, count_lines);
                    break;
                default:
                    if (isdigit(c)) {
                        file_errors << "Error: Invalid tank index in file: " << c << std::endl;
                    } else if (isalpha(c)) {
                        file_errors << "Error: Invalid character in file: " << c << std::endl;
                    } else {
                        file_errors << "Error: Unknown character in file: " << c << std::endl;
                    }
                    file_errors.close();
                    std::cerr << "Error: Invalid character in file: " << c << std::endl;
                    return false;
            }
        }
        ++count_lines;
    }

    if (count_lines < height) {
        file_errors << "Error: Not enough lines in file." << std::endl;
        file_errors.close();
        std::cerr << "Error: Not enough lines in file." << std::endl;
        return false;
    }

    file_board.close();
    displayBoard(); // Display the loaded board
    
    if(count_tanks_for_player1 ==0){
        file_errors << "Error: Not enough tanks for player1"<<std::endl;
        return false;
    }
    else if(count_tanks_for_player2 == 0){
        file_errors << "Error: Not enough tanks for player2"<<std::endl;
    }
    file_errors.close();
    std::cout << "Board loaded successfully." << std::endl;
    return true;
}

void  GameBoard::displayBoard() const {
    std::cout << "Game Board:" << std::endl;
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

void GameBoard::updateShellPosition(Shell *shell, int newX, int newY) {
    if (shell) {
        shell->setX(newX);
        shell->setY(newY);
    }
}



void GameBoard::moveTank(char tank_index, int new_x, int new_y) const {
    if (tank_index == '1' && tank1) {
            (*board)[tank1->getY()][tank1->getX()] = new Empty(tank1->getX(), tank1->getY());
            (*board)[tank1->getPreviousPosition().second][tank1->getPreviousPosition().first] = new Empty(tank1->getPreviousPosition().first, tank1->getPreviousPosition().second);
    
            tank1->setX(new_x);
            tank1->setY(new_y);
    
            (*board)[new_y][new_x] = tank1.get();
       
     //}

    } else if (tank_index == '2' && tank2) {
            (*board)[tank2->getY()][tank2->getX()] = new Empty(tank2->getX(), tank2->getY());
            (*board)[tank2->getPreviousPosition().second][tank2->getPreviousPosition().first] = new Empty(tank2->getPreviousPosition().first, tank2->getPreviousPosition().second);
    
            tank2->setX(new_x);
            tank2->setY(new_y);
    
            (*board)[new_y][new_x] = tank2.get();
        //}

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
bool GameBoard::isCellPassable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if ((*board)[y][x]) {
        CellType c = (*board)[y][x]->getCellType();
        return c == CellType::EMPTY|| c == CellType::TANK1 ||c == CellType::TANK2;
    }
    return false; // Consider null pointers as not passable
}

bool GameBoard::isCellLegal(int x, int y) const{
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if ((*board)[y][x]) {
        CellType c = (*board)[y][x]->getCellType();
        return c != CellType::WALL|| c != CellType::MINE;
    }
    return false; 
}

bool GameBoard::isSteppingWall(int x, int y) const{
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if ((*board)[y][x]) {
        CellType c = (*board)[y][x]->getCellType();
        return c == CellType::WALL;
    }
    return false; 
}
void GameBoard::moveShell(Shell* shell) {
    if (!shell) return; // Always be defensive
    std::cout << "I am in moveShell" << std::endl;
    int old_x = shell->getX();
    int old_y = shell->getY();

    int dx = 0, dy = 0;
    CanonDirection direction = shell->getDirection();

    int speed = 1; // How far a shell moves each time
    std::cout << "Shell speed: " << speed << std::endl;
    switch (direction) {
        case CanonDirection::U:  dy = -speed; break;
        case CanonDirection::UR: dx = speed; dy = -speed; break;
        case CanonDirection::R:  dx = speed; break;
        case CanonDirection::DR: dx = speed; dy = speed; break;
        case CanonDirection::D:  dy = speed; break;
        case CanonDirection::DL: dx = -speed; dy = speed; break;
        case CanonDirection::L:  dx = -speed; break;
        case CanonDirection::UL: dx = -speed; dy = -speed; break;
    }

    // Calculate new position
    int new_x = old_x + dx;
    int new_y = old_y + dy;

    // Wrap around horizontally
    if (new_x < 0) new_x += width;
    if (new_x >= width) new_x -= width;

    // Wrap around vertically
    if (new_y < 0) new_y += height;
    if (new_y >= height) new_y -= height;
    std::cout <<"I am in moveShell after calculating new position" << std::endl;
    //Step 1: clear old
    (*board)[old_y][old_x] = new Empty(old_x, old_y); // Mark as empty

    // Step 2: Move the shell's internal coordinates
    shell->setX(new_x);
    shell->setY(new_y);

    std::cout << "Shell moved to (" << new_x << ", " << new_y << ")" << std::endl;

    // Step 3: Check for collision at new location
    if (!isCellWalkable(new_x, new_y)) {
        std::cout << "Collision detected at (" << new_x << ", " << new_y << ")" << std::endl;
        
        // Clear what's already there
        if ((*board)[new_y][new_x]) {
            std::cout << "Collision with: " << (*board)[new_y][new_x]->getCellType() << std::endl;
            //delete (*board)[new_y][new_x];
            std::cout << "Deleting object at (" << new_x << ", " << new_y << ")" << std::endl;
            (*board)[new_y][new_x] = nullptr; // temporary safety
            std::cout << "Marking as empty." << std::endl;
            //(*board)[new_y][new_x] = nullptr;
            (*board)[new_y][new_x] = new Empty(new_x, new_y); // Mark as empty
        }
        

        std::cout << "Shell destroyed due to collision." << std::endl;
        return; // The shell is considered destroyed - do not reinsert it
    }

    // Step 4: Place the shell in the new location
    if ((*board)[new_y][new_x]) {
        delete (*board)[new_y][new_x];
        (*board)[new_y][new_x] = nullptr;
    }
    (*board)[new_y][new_x] = shell;

    std::cout << "Shell placed at new location (" << new_x << ", " << new_y << ")" << std::endl;
}





ActionType GameBoard::movingAlgorithm(Tank &tank) {
    if (tank.getIndexTank() == '1' && tank1) {
        BfsChaserShir chaser_algorithm;
        //std::cout << "Finished step itai"  << std::endl;
         
        ActionType action =  chaser_algorithm.getNextMove((*this).getGameBoard(), getTank1(), getTank2());
        std::cout<<"Finished step itai"  << std::endl;
        std::cout << "Tank 1 action: " << action << std::endl;
        std::cout << "Finished step for tank1"  << std::endl;
        return action;
    } else if (tank.getIndexTank() == '2' && tank2) {
        Chased chasedAI;
        return chasedAI.decideNextAction(*this, *tank2, *tank1);
        std::cout << "Finished step for tank2"  << std::endl;
    }
    return ActionType::INVALID_ACTION; // Default action if tank is not found
}

int GameBoard::getWidth() const {
    return width;
}

int GameBoard::getHeight() const {
    return height;
}