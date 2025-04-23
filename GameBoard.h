#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "vector"
#include "CellType.h"
#include "CanonDirection.h"
#include "Tank.h"
#include "Shell.h"
#include "Shells.h"
#include <string>
#include <vector>
#include <Wall.h>
#include <algorithm>
class GameBoard {
    int width, height;
    std::vector<std::vector<CellType>> board;
    std::vector<std::vector<Wall>> walls;
    Tank* tank1 = nullptr;
    Tank* tank2 = nullptr;
    Shells shellsList;

public:
    // Initialization
    bool loadBoardFromFile(const std::string& filename);
    void displayBoard();

    // Placement and Movement
    void placeTank(int x, int y, char index_tank, CanonDirection cdir);
    void placeShell(int x, int y);
    void moveTank(char tankIndex, int newX, int newY);
    void shootFromTank(char index_tank, CanonDirection cdir);

    // State Queries
    bool isCellWalkable(int x, int y) const;
    CellType getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    // Tank Accessors
    Tank* getTank1() { return tank1; }
    Tank* getTank2() { return tank2; }

    // Shells access
    const std::vector<Shell>& getShells() const {
        return shellsList.getShellsList();
    }

    std::vector<Shell>& getShells() {
        return shellsList.accessShells(); // Mutable reference to shells
    }

    // Board Accessor
    std::vector<std::vector<CellType>> getBoard() { return board; }

    // Shell Management
    void updateShellPosition(Shell* shell, int x, int y) {
        if (x >= 0 && y >= 0 && y < height && x < width)
            board[y][x] = CellType::SHELL;

        int oldX = shell->getX();
        int oldY = shell->getY();
        if (oldX >= 0 && oldY >= 0 && oldY < height && oldX < width)
            board[oldY][oldX] = CellType::EMPTY;

        shell->updatePosition(x, y);
    }

    void removeShell(Shell& shell) {
        int x = shell.getX();
        int y = shell.getY();

        if (x >= 0 && y >= 0 && y < height && x < width)
            board[y][x] = CellType::EMPTY;

        shellsList.removeShellAt(x, y);
    }

    void addShell(const Shell& shell) {
        shellsList.addShell(shell);
        int x = shell.getX();
        int y = shell.getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::SHELL;
        }
    }

    // Wall Management
    Wall* getWall(int x, int y) {
        if (x >= 0 && y >= 0 && y < height && x < width)
            return &walls[y][x];
        return nullptr;
    }

    void removeWall(Wall* wall) {
        int x = wall->getX();
        int y = wall->getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::EMPTY;
            walls[y][x] = Wall(0, 0); // Reset wall placeholder
        }
    }
    void removeTank(char index_tank) {
        Tank *tank = nullptr;
        if(index_tank == '1') {
            tank = tank1;
        }
        else if(index_tank == '2') {
            tank = tank2;
        }
        int x = tank->getX();
        int y = tank->getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::EMPTY;
        }
    }

    void removeShellAt(int x, int y) {
        for(Shell shell : shellsList.getShellsList()) {
            if(shell.getX() == x && shell.getY() == y) {
                board[y][x] = CellType::EMPTY;
                break;
            }
        }
    }
    void moveShell(Shell* shell);
    ActionType movingAlgorithm(Tank& tank);
};

#endif // GAMEBOARD_H
