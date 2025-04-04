#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <vector>
#include "CellType.h"
#include "CanonDirection.h"
#include "Tank.h"
#include "Shell.h"
#include <string>

class GameBoard {
    int width, height;
    std::vector<std::vector<CellType>> board;
    Tank* tank1 = nullptr;
    Tank* tank2 = nullptr;
public:
    bool loadBoardFromFile(const std::string& filename);
    void displayBoard();
    void placeTank(int x, int y, char index_tank, CanonDirection cdir);
    void placeShell(int x, int y);
    void moveTank(char tankIndex, int newX, int newY);
    void shootFromTank(char index_tank, CanonDirection cdir);
};

#endif //GAMEBOARD_H
