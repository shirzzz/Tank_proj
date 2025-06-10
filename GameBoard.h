#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "vector"
#include "CellType.h"
#include "Empty.h"
#include "CanonDirection.h"
#include "Tank.h"
#include "Shell.h"
#include <string>
#include <vector>
#include "Wall.h"
#include <algorithm>
#include <memory>
#include <iostream>

class GameBoard {
    int width, height;
    std::vector<std::vector<std::shared_ptr<Shape>>> board;
    std::vector<std::vector<bool>> is_mine; // For mines, if needed
    // std::shared_ptr<Tank> tank1;
    // std::shared_ptr<Tank>tank2;
    int num_walls = 0;
    std::vector<Shell> shells;
    int max_steps = 0;
    int num_shells = 0;

    //Class methods which are only used in this class
    // Placement and Movement
    void placeTank(int x, int y, char index_tank, CanonDirection cdir);
    void placeShell(int x, int y) const;

    void shootFromTank(char index_tank, CanonDirection cdir);

public:
GameBoard(int width, int height, std::vector<std::vector<std::shared_ptr<Shape>>> game_board, std::vector<std::vector<bool>> is_mines) : width(width), height(height) {
    board.resize(height, std::vector<std::shared_ptr<Shape>>(width));
    is_mine = std::move(is_mines); // Initialize mine grid
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width;x++){
            board[y][x] = game_board[y][x];
        }
    }
}
    ~GameBoard() = default;

    bool loadBoardFromFile(std::istream& file_board, std::string filename);
    void displayBoard() const;
    void displayBoard(std::ofstream& file) const;

    // State Queries
    bool isCellWalkable(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    // // Tank Accessors
    // std::shared_ptr<Tank> getTank1() const { return tank1; }
    // std::shared_ptr<Tank> getTank2() const { return tank2; }

    std::vector<Shell>&  getShells () {
        return shells; // Mutable reference to shells
    }

    const std::vector<Shell>&  getShells () const {
        return shells; // Mutable reference to shells
    }

    std::shared_ptr<Shape> getCell(int x, int y) {
        if (x >= 0 && y >= 0 && y < height && x < width)
            return board[y][x];
        return nullptr;
    }

    bool removeShellAt(int x, int y) ;
    
    // Shell Management
    void updateShellPosition(Shell* shell, int x, int y);

    void removeShell(Shell& shell);
    void displayGame() const;
    void addShell(const Shell& shell);
    
    void removeWall(Wall* wall);
    void removeTank(std::shared_ptr<Tank>tank);

    void removeShellAtfromBoard(int x, int y);
    void moveShell(Shell* shell);
    //ActionRequest movingAlgorithm(Tank& tank);
    void setCell(int x, int y, std::shared_ptr<Shape> shape);
    
    bool isCellPassable(int x, int y) const;
    int getNumWalls() const { return num_walls; }
    void setNumWalls(int num) { num_walls = num; }
    bool isCellLegal(int x, int y) const;
    bool isSteppingWall(int x, int y) const;
    bool isSteppingMine(int x, int y) const;
    int getMaxSteps() const { return max_steps; }
    void moveTank(std::shared_ptr<Tank>& tank, int newX, int newY);
    std::pair<int, int> getNextPosition(int x, int y, CanonDirection direction) const;
    void bombMine(int x, int y) {
        if (x >= 0 && y >= 0 && y < height && x < width) {
            is_mine[y][x] = false; // Remove the mine
            board[y][x] = std::make_shared<Empty>(x, y); // Reset cell to empty
        }
    }
};

#endif // GAMEBOARD_H
