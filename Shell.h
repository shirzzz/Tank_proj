//
// Created by Owner on 01/04/2025.
//

#ifndef SHELL_H
#define SHELL_H
#include "CanonDirection.h"
#include "shape.h"
#include "GameBoard.h"

class Shell : public Shape {
    int speed;
public:
    Shell(int x, int y, CanonDirection cdir);

    void moveShell(GameBoard& game_board) {
        int dx = 0, dy = 0;
        switch (direction) {
            case CanonDirection::U:
                dy = speed;
                break;
            case CanonDirection::UR:
                dx = speed;
                dy = speed;
                break;
            case CanonDirection::R:
                dx = speed;
                break;
            case CanonDirection::DR:
                dx = speed;
                dy = -speed;
                break;
            case CanonDirection::D:
                dy = -speed;
                break;
            case CanonDirection::DL:
                dx = -speed;
                dy = -speed;
                break;
            case CanonDirection::L:
                dx = -speed;
                break;
            case CanonDirection::UL:
                dx = -speed;
                dy = speed;
                break;
        }
        x += dx;
        y += dy;
        // Update the shell's position on the game board
        game_board.updateShellPosition(this, x, y);
        // Check for collision with walls or tanks
        if (!game_board.isCellWalkable(x, y)) {
            // Handle collision (e.g., remove shell)
        }
    }

};






#endif //SHELL_H
