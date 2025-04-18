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
class GameBoard {
    int width, height;
    std::vector<std::vector<CellType>> board;
    std::vector<Shell> shells;
    std::vector<std::vector<Wall>> walls; // Assuming Wall is a class representing walls
    Tank* tank1 = nullptr;
    Tank* tank2 = nullptr;
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
    std::vector<Shell> getShells() { return shells; }
    Tank* getTank1() { return tank1; }
    Tank* getTank2() { return tank2; }
    std::vector<std::vector<CellType>> getBoard() { return board; }
    void updateShellPosition(Shell* shell, int x, int y) {
        // Update the shell's position on the game board
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::SHELL; // Assuming SHELL1 for simplicity
        }
        int oldX = shell->getX();
        int oldY = shell->getY();
        if (oldX >= 0 && oldY >= 0 && oldY < height && oldX < width) {
            board[oldY][oldX] = CellType::EMPTY; // Clear the old position
        }
        shell->updatePosition(x, y); // Update the shell's position in the Shell object

    }
    void removeShell(Shell& shell) {
        int x = shell.getX();
        int y = shell.getY();

        // Clear the cell on the board
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::EMPTY;
        }

        // Remove the shell from the vector of shells
        shells.erase(
            std::remove_if(shells.begin(), shells.end(), [&](const Shell& s) {
                return s.getX() == x && s.getY() == y;
            }),
            shells.end()
        );
    }
    Wall* getWall(int x, int y) {
        // Assuming walls are stored in a 2D vector
        if (x >= 0 && y >= 0 && y < height && x < width) {
            return &walls[y][x]; // Return the wall at the specified position
        }
        return nullptr; // Return null if out of bounds
    }
    void removeWall(Wall* wall) {
        // Assuming walls are stored in a 2D vector
        int x = wall->getX();
        int y = wall->getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::EMPTY; // Clear the position
            walls[y][x] = Wall(0, 0); // Assuming Wall has a constructor that sets it to empty
        }
    }
    void addShell(const Shell& shell) {
        shells.push_back(shell);
        int x = shell.getX();
        int y = shell.getY();
        if (x >= 0 && y >= 0 && y < height && x < width) {
            board[y][x] = CellType::SHELL; // Assuming SHELL1 for simplicity
        }
    }
};

#endif //GAMEBOARD_H
