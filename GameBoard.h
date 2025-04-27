#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "vector"
#include "CellType.h"
#include "Empty.h"
#include "CanonDirection.h"
#include "Tank.h"
#include "Shell.h"
#include "Shells.h"
#include <string>
#include <vector>
#include "Wall.h"
#include <algorithm>
#include <memory>

class GameBoard {
    int width, height;
    std::shared_ptr<std::vector<std::vector<Shape*>>> board;
    std::shared_ptr<Tank> tank1;
    std::shared_ptr<Tank>tank2;
    int num_walls = 0;
    //Shells shellsList;
    std::vector<Shell> shells;

public:
    GameBoard(int width, int height) : width(width), height(height) {
        board = std::make_shared<std::vector<std::vector<Shape*>>>(height, std::vector<Shape*>(width, new Empty(-1, -1)));
    }
    // GameBoard(int width, int height) : width(width), height(height) {
    //     board = std::make_shared<std::vector<std::vector<Shape*>>>(height, std::vector<Shape*>(width, new Empty(-1, -1)));
    // }
    // GameBoard(std::string filename){
    //     if(loadBoardFromFile(filename)){
    //     } else {
    //         throw std::runtime_error("Failed to load board from file");
    //     };
    // }
    ~GameBoard() = default;
    //~GameBoard();
    // GameBoard(const GameBoard& other) : width(other.width), height(other.height) {
    //     board = std::make_shared<std::vector<std::vector<Shape*>>>(height, std::vector<Shape*>(width, nullptr));
    //     for (int i = 0; i < height; ++i) {
    //         for (int j = 0; j < width; ++j) {
    //             if (other.board->at(i).at(j)) {
    //                 board->at(i).at(j) = other.board->at(i).at(j)->clone();
    //             }
    //         }
    //     }
    // }
    bool loadBoardFromFile(std::ifstream& file_board, std::string filename);
    void displayBoard() const;
    // Placement and Movement
    void placeTank(int x, int y, char index_tank, CanonDirection cdir);
    void placeShell(int x, int y) const;
    void moveTank(char tankIndex, int newX, int newY) const;
    void shootFromTank(char index_tank, CanonDirection cdir);

    // State Queries
    bool isCellWalkable(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    // Tank Accessors
    std::shared_ptr<Tank> getTank1() const { return tank1; }
    std::shared_ptr<Tank> getTank2() const { return tank2; }

    std::vector<Shell>&  getShells () {
        return shells; // Mutable reference to shells
    }

    const std::vector<Shell>&  getShells () const {
        return shells; // Mutable reference to shells
    }

    // Board Accessor
    std::shared_ptr<GameBoard> getGameBoard() const {
        return std::make_shared<GameBoard>(*this);
    }
    Shape* getCell(int x, int y) const {
        if (x >= 0 && y >= 0 && y < height && x < width)
            return (*board)[y][x];
        return nullptr;
    }

    bool removeShellAt(int x, int y) {
        std::cout << "Attempting to remove shell at (" << x << ", " << y << ")\n";
        
        // Find the element that matches the given coordinates
        auto it = std::remove_if(shells.begin(), shells.end(), [&](Shell& s) {
            int shell_x = s.getX();
            int shell_y = s.getY();
            std::cout << "Checking shell at (" << shell_x << ", " << shell_y << ")\n";
            return (shell_x == x && shell_y == y);
        });
    
        // Check if the element was found
        if (it != shells.end()) {
            std::cout << "Found matching shell. Erasing...\n";
            // Now erase the element from the vector
            shells.erase(it, shells.end());
            std::cout << "Shell removed successfully.\n";
            return true;  // Indicate successful removal
        } else {
            std::cout << "No matching shell found.\n";
        }
    
        return false;  // No matching element found
    }
    
    
    

    // Shell Management
    void updateShellPosition(Shell* shell, int x, int y);

    void removeShell(Shell& shell) {
        int x = shell.getX();
        int y = shell.getY();

        if (x >= 0 && y >= 0 && y < height && x < width)
            (*board)[y][x] = new Empty(x, y); // Reset cell to empty

        removeShellAt(x, y);
        removeShellAtfromBoard(x, y);
    }

    void addShell(const Shell& shell) {
        // 🔵 [NO CHANGE] First, add the Shell copy to Shells list
        //shellsList.addShelltoList(shell);
        std::cout << "Adding shell at (" << shell.getX() << ", " << shell.getY() << ")" << std::endl;
        std::cout << "Shells size: " << shells.size() << std::endl;
        shells.push_back(shell);
        std::cout << "Shell added to list." << std::endl;
        std::cout << "Shells size her: " <<shells.size() << std::endl;
        int x = shell.getX();
        int y = shell.getY();
    
        // 🔵 [CHANGE] Carefully update the board
        // if ((*board)[y][x]) {
        //     delete (*board)[y][x];
        //     (*board)[y][x] = nullptr; // Safety after delete
        // }
    
        // 🔵 [CHANGE] Set a new Shell into the board
        (*board)[y][x] = new Shell(x, y, shell.getDirection());
    }
    


    void removeWall(Wall* wall) {
        int x = wall->getX();
        int y = wall->getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            (*board)[y][x] = new Empty(x, y); // Reset cell to empty
        }
    }
    void removeTank(std::shared_ptr<Tank>tank) {
        int x = tank->getX();
        int y = tank->getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            (*board)[y][x] = new Empty(x, y); // Reset cell to empty
        }
    }

    void removeShellAtfromBoard(int x, int y) {
        for(Shell shell : shells) {
            if(shell.getX() == x && shell.getY() == y) {
                (*board)[y][x] = new Empty(x, y); // Reset cell to empty
                break;
            }
        }
    }
    void moveShell(Shell* shell);
    ActionType movingAlgorithm(Tank& tank);
    void setCell(int x, int y, Shape* shape) {
        if (x >= 0 && y >= 0 && y < height && x < width) {
            (*board)[y][x] = shape;
        }
    }
    
    bool isCellPassable(int x, int y) const;
    int getNumWalls() const { return num_walls; }
    void setNumWalls(int num) { num_walls = num; }

};

#endif // GAMEBOARD_H
