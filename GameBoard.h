#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "vector"
#include "CellType.h"
#include "CanonDirection.h"
#include "Tank.h"
#include "Shell.h"
#include <string>
#include <vector>
#include <Wall.h>
#include <algorithm>
#include "Shells.h"

class GameBoard {
    int width, height;
    std::vector<std::vector<CellType>> board;
    std::vector<std::vector<Wall>> walls;
    Tank* tank1 = nullptr;
    Tank* tank2 = nullptr;
    Shells shellsList;

public:
    bool loadBoardFromFile(const std::string& filename);
    void displayBoard();
    void placeTank(int x, int y, char index_tank, CanonDirection cdir);
    void placeShell(int x, int y);
    void moveTank(char tankIndex, int newX, int newY);
    void shootFromTank(char index_tank, CanonDirection cdir);

    bool isCellWalkable(int x, int y) const;
    CellType getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    const std::vector<Shell>& getShells() const {
        return shellsList.getShellsList();
    }

    Tank* getTank1() { return tank1; }
    Tank* getTank2() { return tank2; }

    std::vector<std::vector<CellType>> getBoard() { return board; }

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
            walls[y][x] = Wall(0, 0); // Reset wall
        }
    }

    void addShell(const Shell& shell) {
        shellsList.addShell(shell);
        int x = shell.getX();
        int y = shell.getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::SHELL;
        }
    }
};

#endif // GAMEBOARD_H
