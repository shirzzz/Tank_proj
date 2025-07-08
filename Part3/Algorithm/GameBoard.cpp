#include "GameBoard.h"
#include "CellType.h"
#include "../UserCommon/Tank.h"
#include "../UserCommon/Shell.h"
#include "../UserCommon/CanonDirection.h"
#include "regex"
#include "../UserCommon/Shape.h"
#include "Empty.h"
#include "../UserCommon/Wall.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "Chased.h"
#include "../UserCommon/Mine.h"
#include "BfsChaserShir.h"

/**
 * @brief Loads game board configuration from input stream
 * @param file_board Input stream containing board data
 * @param filename Name of the source file for error reporting and logging
 * @return true if board loaded successfully, false otherwise
 * 
 * Design Decision: The filename parameter is used for enhanced error reporting
 * and debugging. While the actual file reading is done through the input stream,
 * having the filename allows us to:
 * 1. Provide more informative error messages to users
 * 2. Create better logging and debugging output
 * 3. Support future features like configuration file validation
 * 4. Maintain consistency with file-based error reporting patterns
 * 
 * This approach separates file I/O concerns (handled by caller) from
 * data parsing (handled by this method) while still maintaining context
 * about the data source for error reporting purposes.
 */
namespace Algorithm_211466123_212399455{
void  GameBoard::displayBoard() const {
    std::cout << "Game Board:" << std::endl;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (board[y][x]) {
                switch (board[y][x]->getCellType()) {
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

void  GameBoard::displayBoard(std::ofstream& file) const {
    std::cout << "Game Board:" << std::endl;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (board[y][x]) {
                switch (board[y][x]->getCellType()) {
                    case CellType::EMPTY:
                        file << " ";
                        break;
                    case CellType::WALL:
                        file << "#";
                        break;
                    case CellType::TANK1:
                        file << "1";
                        break;
                    case CellType::TANK2:
                        file << "2";
                        break;
                    case CellType::SHELL:
                        file << "o";
                        break;
                    case CellType::MINE:
                        file << "@";
                        break;
                    default:
                        file << " "; // Handle unknown cell types
                        break;
                }
            } else {
                file << " "; // Handle null pointer case
            }
        }
        file << std::endl;
    }
}

void GameBoard::updateShellPosition(Shell *shell, int new_x, int new_y) {
    if (shell) {
        shell->setX(new_x);
        shell->setY(new_y);
    }
}

void GameBoard::moveTank(std::shared_ptr<Tank>& current_tank,  int new_x, int new_y){
    if(current_tank){
        int old_x = current_tank->getX();
        int old_y = current_tank->getY();
        int prev_x = current_tank->getPreviousPosition().first;
        int prev_y = current_tank->getPreviousPosition().second;
        board[old_y][old_x] = std::make_shared<Empty>(old_x, old_y);

        current_tank->setX(new_x);
        current_tank->setY(new_y);
        if(isCellWalkable(new_x, new_y)){
            if(old_x != prev_x || old_y != prev_y){
                board[prev_y][prev_x] = std::make_shared<Empty>(prev_x, prev_y);
            }
            board[new_y][new_x] = std::make_shared<Tank>(new_x, new_y, current_tank->getIndexTank());
        }
        
    } else{
        std::cerr << "Error:Attempted to move a tank that does not exist (index: "<< current_tank->getIndexTank()<<")" <<std::endl;
    }
}

bool GameBoard::isCellWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if (board[y][x]) {
        CellType c = board[y][x]->getCellType();
        return c != CellType::MINE && c != CellType::WALL && c != CellType::TANK1 && c != CellType::TANK2 && c != CellType::SHELL;
    }
    return false; // Consider null pointers as not walkable
}
bool GameBoard::isCellPassable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if (board[y][x]) {
        CellType c = board[y][x]->getCellType();
        return c == CellType::EMPTY|| c == CellType::TANK1 ||c == CellType::TANK2;
    }
    return false; // Consider null pointers as not passable
}

bool GameBoard::isCellLegal(int x, int y) const{
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if (board[y][x]) {
        CellType c = board[y][x]->getCellType();
        return c != CellType::WALL|| c != CellType::MINE;
    }
    return false; 
}

bool GameBoard::isSteppingWall(int x, int y) const{
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if (board[y][x]) {
        CellType c = board[y][x]->getCellType();
        return c == CellType::WALL;
    }
    return false; 
}

bool GameBoard::isSteppingMine(int x, int y) const {
        if (x >= 0 && y >= 0 && y < height && x < width) {
            return is_mine[y][x]; // Check if the cell is a mine
        }
        return false; // Out of bounds
    }

std::pair<int, int> GameBoard::getNextPosition(int x, int y, CanonDirection direction) const {
    int dx = 0, dy = 0;
    switch (direction) {
        case CanonDirection::U:  dy = -1; break;
        case CanonDirection::UR: dx = 1; dy = -1; break;
        case CanonDirection::R:  dx = 1; break;
        case CanonDirection::DR: dx = 1; dy = 1; break;
        case CanonDirection::D:  dy = 1; break;
        case CanonDirection::DL: dx = -1; dy = 1; break;
        case CanonDirection::L:  dx = -1; break;
        case CanonDirection::UL: dx = -1; dy = -1; break;
    }
    return { (x + dx + width) % width, (y + dy + height) % height };
}

void GameBoard::moveShell(Shell* shell) {
    if (!shell) return; // Always be defensive
    int old_x = shell->getX();
    int old_y = shell->getY();
    int dx = 0, dy = 0;
    CanonDirection direction = shell->getDirection();

    int speed = 1; // How far a shell moves each time
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
    //Step 1: clear old
    board[old_y][old_x] = std::make_shared<Empty>(old_x, old_y); // Mark as empty

    // Step 2: Move the shell's internal coordinates
    shell->setX(new_x);
    shell->setY(new_y);

    // Step 3: Check for collision at new location
    if (isCellWalkable(new_x, new_y)) {
        board[new_y][new_x] = std::make_shared<Shell>(*shell);
    }
    if(is_mine[old_y][old_x]) {
        board[old_y][old_x] = std::make_shared<Mine>(old_x, old_y); // Reset cell to mine if it was a mine
    }
}

int GameBoard::getWidth() const {
    return width;
}

int GameBoard::getHeight() const {
    return height;
}

bool GameBoard::removeShellAt(int x, int y) {
    // Find the element that matches the given coordinates
    auto it = std::remove_if(shells.begin(), shells.end(), [&](Shell& s) {
        int shell_x = s.getX();
        int shell_y = s.getY();
        return (shell_x == x && shell_y == y);
    });

    // Check if the element was found
    if (it != shells.end()) {
        // Now erase the element from the vector
        shells.erase(it, shells.end());
        return true;  // Indicate successful removal
    }
    return false;  // No matching element found
}

void GameBoard::removeShell(Shell& shell) {
    int x = shell.getX();
    int y = shell.getY();

    if (x >= 0 && y >= 0 && y < height && x < width)
        board[y][x] = std::make_shared<Empty>(x, y); // Reset cell to empty

    removeShellAt(x, y);
    removeShellAtfromBoard(x, y);
}

void GameBoard::addShell(const Shell& shell) {
    shells.push_back(shell);
    int x = shell.getX();
    int y = shell.getY();
    board[y][x] = std::make_shared<Shell>(x, y, shell.getDirection());
}

void GameBoard::removeWall(Wall* wall) {
    int x = wall->getX();
    int y = wall->getY();
    if (x >= 0 && y >= 0 && y < height && x < width) {
        board[y][x] = std::make_shared<Empty>(x, y); // Reset cell to empty
    }
}
void GameBoard::removeTank(std::shared_ptr<Tank>tank) {
    int x = tank->getX();
    int y = tank->getY();
    if (x >= 0 && y >= 0 && y < height && x < width) {
        board[y][x] = std::make_shared<Empty>(x, y); // Reset cell to empty
    }
}

void GameBoard::removeShellAtfromBoard(int x, int y) {
    for(Shell shell : shells) {
        if(shell.getX() == x && shell.getY() == y) {
            board[y][x] = std::make_shared<Empty>(x, y); // Reset cell to empty
            break;
        }
    }
}
void GameBoard::setCell(int x, int y, std::shared_ptr<Shape> shape) {
    if (x >= 0 && y >= 0 && y < height && x < width) {
        board[y][x] = std::make_shared<Shape>(*shape);
    }
}
}

