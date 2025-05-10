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
    std::shared_ptr<Tank> tank1;
    std::shared_ptr<Tank>tank2;
    int num_walls = 0;
    std::vector<Shell> shells;
    int max_steps = 0;
    int num_shells = 0;
    Player1 player1;
    Player2 player2;

public:
GameBoard(int width, int height) : width(width), height(height){
    board.resize(height, std::vector<std::shared_ptr<Shape>>(width));
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width;x++){
            board[y][x] = std::make_shared<Empty> (x,y);
        }
    }
}
    ~GameBoard() = default;

    bool loadBoardFromFile(std::istream& file_board, std::string filename);
    void displayBoard() const;
    // Placement and Movement
    void placeTank(int x, int y, char index_tank, CanonDirection cdir);
    void placeShell(int x, int y) const;
    void moveTank(char tankIndex, int newX, int newY);
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

    // // Board Accessor
    std::shared_ptr<GameBoard> getGameBoard() const {
        return std::make_shared<GameBoard>(*this);
    }
    const Shape* getCell(int x, int y) const {
        if (x >= 0 && y >= 0 && y < height && x < width)
            return board[y][x].get();
        return nullptr;
    }

    bool removeShellAt(int x, int y) ;
    
    // Shell Management
    void updateShellPosition(Shell* shell, int x, int y);

    void removeShell(Shell& shell);

    void addShell(const Shell& shell);
    
    void removeWall(Wall* wall);
    void removeTank(std::shared_ptr<Tank>tank);

    void removeShellAtfromBoard(int x, int y);
    void moveShell(Shell* shell);
    ActionType movingAlgorithm(Tank& tank);
    void setCell(int x, int y, std::shared_ptr<Shape> shape);
    
    bool isCellPassable(int x, int y) const;
    int getNumWalls() const { return num_walls; }
    void setNumWalls(int num) { num_walls = num; }
    bool isCellLegal(int x, int y) const;
    bool isSteppingWall(int x, int y) const;
    bool isSteppingMine(int x, int y) const;
    Player1 getPlayer1() const { return player1; }
    Player2 getPlayer2() const { return player2; }
    void setPlayer1(const Player1& player) { player1 = player; }
    void setPlayer2(const Player2& player) { player2 = player; }
};

#endif // GAMEBOARD_H
