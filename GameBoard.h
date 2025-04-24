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
    Shells shellsList;

public:
    // Initialization
    bool loadBoardFromFile(const std::string& filename);
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

    // Shells access
    const std::vector<Shell>& getShells() const {
        return shellsList.getShellsList();
    }

    std::vector<Shell>& getShells() {
        return shellsList.accessShells(); // Mutable reference to shells
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

    // Shell Management
    void updateShellPosition(Shell* shell, int x, int y);

    void removeShell(Shell& shell) {
        int x = shell.getX();
        int y = shell.getY();

        if (x >= 0 && y >= 0 && y < height && x < width)
            (*board)[y][x] = new Empty(x, y); // Reset cell to empty

        shellsList.removeShellAt(x, y);
    }

    void addShell(const Shell& shell) {
        shellsList.addShell(shell);
        int x = shell.getX();
        int y = shell.getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            (*board)[y][x] = new Shell(shell.getX(), shell.getY(), shell.getDirection());
        }
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

    void removeShellAt(int x, int y) {
        for(Shell shell : shellsList.getShellsList()) {
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

};

#endif // GAMEBOARD_H
