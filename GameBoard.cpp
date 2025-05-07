#include "GameBoard.h"
#include "CellType.h"
#include "Tank.h"
#include "Shell.h"
#include "CanonDirection.h"
#include "regex"
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

bool GameBoard::loadBoardFromFile(std::istream& file_board, std::string filename) {
    
    int count_tanks_for_player1 = 0; //counts how many tanks player1 has
    int count_tanks_for_player2 = 0; //counts how many tanks player2 has
    std::ofstream file_errors("input_errors.txt");
    std::cout << "Opening file for writing input errors." << std::endl;
    if (!file_errors) {
        file_errors << "Error: Failed to open file for writing input errors.\n" << std::endl;
        file_errors.close();
        std::cerr << "Failed to open file for writing input errors." << std::endl;
        return false;
    }
    std::regex pattern_max_steps(R"(MaxSteps\s*:\s*(\d+))");
    std::regex pattern_num_shells(R"(NumShells\s*:\s*(\d+))");
    std::regex pattern_rows(R"(Rows\s*:\s*(\d+))");
    std::regex pattern_cols(R"(Cols\s*:\s*(\d+))");
    // **I think we need to change this part ***
    player1 =  player1(1, 0, 0, 0, 0);
    player2 = player2(2, 0, 0, 0, 0);
    for(int i = 0; i < 4; i++){
        std::string line;
        std::getline(file_board, line);
        if (line.empty()) {
            continue; // Skip empty lines
        }
        std::smatch match;
        if (std::regex_search(line, match, pattern_max_steps)) {
            max_steps = std::stoi(match[1]);
            std::cout << "MaxSteps: " << max_steps << std::endl;
        } else if (std::regex_search(line, match, pattern_num_shells)) {
            num_shells = std::stoi(match[1]);
            std::cout << "NumShells: " << num_shells << std::endl;
        } else if (std::regex_search(line, match, pattern_rows)) {
            height = std::stoi(match[1]);
            std::cout << "Rows: " << height << std::endl;
        } else if (std::regex_search(line, match, pattern_cols)) {
            width = std::stoi(match[1]);
            std::cout << "Cols: " << width << std::endl;
        }
        else {
            file_errors << "Error: Invalid line format in input file: " << line << std::endl;
            std::cerr << "Invalid line format in input file: " << line << std::endl;
            return false;
        }
    }
    if (width <= 0 || height <= 0) {
        file_errors << "Error: Invalid board dimensions." << std::endl;
        std::cerr << "Invalid board dimensions." << std::endl;
        return false;
    }
    for(int i = 0; i < height; i++){
        std::string line;
        if()
        std::getline(file_board, line);
        if(file_board.eof()){
            line = " "*width; // Fill the rest of the board with empty spaces
        }
        if (line.empty()) {
            continue; // Skip empty lines
        }
        for(int j = 0; j < width; j++){
            char c = ' ';
        
            if(j < line.size()){
                c = line[j];
            }
            if (c == '1') {
                count_tanks_for_player1++;
                std::shared_ptr<Tank>& current_tank = std::make_shared<Tank>(j, i, '1');
                board[i][j] = current_tank;
                player1.addTank(current_tank);
            }
            else if( c== '2') {
                count_tanks_for_player2++;
                std::shared_ptr<Tank>& current_tank = std::make_shared<Tank>(j, i, '2');
                board[i][j] = current_tank;
                player2.addTank(current_tank);
            }
            else if (c == ' ') {
                board[i][j] = std::make_shared<Empty>(j, i);
            }
            else if (c == '#') {
                board[i][j] = std::make_shared<Wall>(j, i);
            }
            else if (c == '@') {
                board[i][j] = std::make_shared<Mine>(j, i);
            }
            else if (c == 'o') {
                board[i][j] = std::make_shared<Shell>(j, i);
            }
            else {
                file_errors << "Error: Invalid character in input file: " << c << std::endl;
                std::cerr << "Invalid character in input file: " << c << std::endl;
                return false;
            }
        }
    }    

    displayBoard(); // Display the loaded board

    file_errors.close();
    std::cout << "Board loaded successfully." << std::endl;
    return true;
}

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

void GameBoard::updateShellPosition(Shell *shell, int new_x, int new_y) {
    if (shell) {
        shell->setX(new_x);
        shell->setY(new_y);
    }
}

void GameBoard::moveTank(char tank_index, int new_x, int new_y){
    std::shared_ptr<Tank>& current_tank = (tank_index =='1') ? tank1 : tank2;
    if(current_tank){
        int old_x = current_tank->getX();
        int old_y = current_tank->getY();
        int prev_x = current_tank->getPreviousPosition().first;
        int prev_y = current_tank->getPreviousPosition().second;
        board[old_y][old_x] = std::make_shared<Empty>(old_x, old_y);
        if(old_x != prev_x || old_y != prev_y){
            board[prev_y][prev_x] = std::make_shared<Empty>(prev_x, prev_y);
        }
        current_tank->setX(new_x);
        current_tank->setY(new_y);
        board[new_y][new_x] = std::make_shared<Tank>(new_x, new_y, current_tank->getIndexTank());
    } else{
        std::cerr << "Error:Attempted to move a tank that does not exist (index: "<< tank_index<<")" <<std::endl;
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

bool GameBoard::isSteppingMine(int x, int y) const{
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    if (board[y][x]) {
        CellType c = board[y][x]->getCellType();
        return c == CellType::MINE;
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
    board[old_y][old_x] = std::make_shared<Empty>(old_x, old_y); // Mark as empty

    // Step 2: Move the shell's internal coordinates
    shell->setX(new_x);
    shell->setY(new_y);

    std::cout << "Shell moved to (" << new_x << ", " << new_y << ")" << std::endl;

    // Step 3: Check for collision at new location
    if (!isCellWalkable(new_x, new_y)) {
        std::cout << "Collision detected at (" << new_x << ", " << new_y << ")" << std::endl;
        
        // Clear what's already there
        if (board[new_y][new_x]) {
            std::cout << "Collision with: " << board[new_y][new_x]->getCellType() << std::endl;
            board[new_y][new_x] = std::make_shared<Empty>(new_x, new_y); // Mark as empty
        }
        std::cout << "Shell destroyed due to collision." << std::endl;
        return; // The shell is considered destroyed - do not reinsert it
    }

    // Step 4: Place the shell in the new location
    board[new_y][new_x] = std::make_shared<Shell>(*shell);
    std::cout << "Shell placed at new location (" << new_x << ", " << new_y << ")" << std::endl;
}

ActionType GameBoard::movingAlgorithm(Tank &tank) {
    if (tank.getIndexTank() == '1' && tank1) {
        BfsChaserShir chaser_algorithm;
         
        ActionType action =  chaser_algorithm.getNextMove((*this).getGameBoard(), getTank1(), getTank2());
        return action;
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

bool GameBoard::removeShellAt(int x, int y) {
    // Find the element that matches the given coordinates
    auto it = std::remove_if(shells.begin(), shells.end(), [&](Shell& s) {
        int shell_x = s.getX();
        int shell_y = s.getY();
        return (shell_x == x && shell_y == y);
    });

    // Check if the element was found
    if (it != shells.end()) {
        std::cout << "Found matching shell. Erasing...\n";
        // Now erase the element from the vector
        shells.erase(it, shells.end());
        return true;  // Indicate successful removal
    } else {
        std::cout << "No matching shell found.\n";
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