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

private:
    std::pair<int, int> previousPosition;

public:
    Shell(int x, int y, CanonDirection cdir);
    void storePreviousPosition() { previousPosition = {x, y}; }
    std::pair<int, int> getPreviousPosition() const { return previousPosition; }

    void moveShell(GameBoard& game_board) {
        int dx = 0, dy = 0;

        switch (direction) {
            case CanonDirection::U:  dy = speed; break;
            case CanonDirection::UR: dx = speed; dy = speed; break;
            case CanonDirection::R:  dx = speed; break;
            case CanonDirection::DR: dx = speed; dy = -speed; break;
            case CanonDirection::D:  dy = -speed; break;
            case CanonDirection::DL: dx = -speed; dy = -speed; break;
            case CanonDirection::L:  dx = -speed; break;
            case CanonDirection::UL: dx = -speed; dy = speed; break;
        }

        int x_moved = x + dx;
        int y_moved = y + dy;

        // Get board size
        int board_width = game_board.getWidth();
        int board_height = game_board.getHeight();

        // Wrap around horizontally
        if (x_moved < 0) x_moved += board_width;
        if (x_moved >= board_width) x_moved -= board_width;

        // Wrap around vertically
        if (y_moved < 0) y_moved += board_height;
        if (y_moved >= board_height) y_moved -= board_height;

        // Update position
        x = x_moved;
        y = y_moved;

        // Update the shell's position on the game board
        game_board.updateShellPosition(this, x, y);

        // Check for collision with walls or tanks
        if (!game_board.isCellWalkable(x, y)) {
            // Handle collision (e.g., remove shell)
        }
        
    }
};







#endif //SHELL_H
