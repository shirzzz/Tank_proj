#include "Chased.h"
#include "DirectionUtils.h"
#include "ActionType.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>

// Check if any shell will move into the tile in front of the tank
bool Chased::isDangerAhead(const Tank& tank,const GameBoard& board) {
    std::pair<int, int> dir = directionToVector(tank.getCanonDirection());
    int dx = dir.first;
    int dy = dir.second;
    int fx = tank.getX() + dx;
    int fy = tank.getY() + dy;

    for (const Shell& shell : board.getShells()) {
        int sx = shell.getX();
        int sy = shell.getY();
        auto dir = directionToVector(shell.getDirection());
        int sdx = dir.first;
        int sdy = dir.second;

        for (int step = 0; step < 5; ++step) {
            if (sx == fx && sy == fy) return true;

            sx += sdx;
            sy += sdy;
            if (sx < 0 || sx >= board.getWidth() || sy < 0 || sy >= board.getHeight())
                break;
        }
    }

    return false;
}

// Helper function: check if opponent is aligned with self
bool isAlignedHorizontally(const Tank& self, const Tank& opponent) {
    return self.getY() == opponent.getY();
}

bool isAlignedVertically(const Tank& self, const Tank& opponent) {
    return self.getX() == opponent.getX();
}

// Helper: check if cannon direction matches direction to opponent
bool isFacingOpponent(const Tank& self, const Tank& opponent) {
    int dx = opponent.getX() - self.getX();
    int dy = opponent.getY() - self.getY();

    if (dx == 0 && dy == 0) return false; // Same position (somehow?)

    CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
    return self.getCanonDirection() == dirToOpponent;
}

// Compute best rotation toward the opponent's position
ActionType rotateToward(const Tank& self, const Tank& opponent) {
    int dx = opponent.getX() - self.getX();
    int dy = opponent.getY() - self.getY();
    CanonDirection desiredDir = getDirectionFromDelta(dx, dy);
    CanonDirection currentDir = self.getCanonDirection();

    if (currentDir == desiredDir) return ActionType::SHOOT;

    int curIdx = static_cast<int>(currentDir);
    int desIdx = static_cast<int>(desiredDir);
    int diff = (desIdx - curIdx + 8) % 8;

    if (diff == 1 || diff == 2) return ActionType::ROTATE_EIGHTH_RIGHT;
    if (diff == 6 || diff == 7) return ActionType::ROTATE_EIGHTH_LEFT;
    if (diff == 3 || diff == 4) return ActionType::ROTATE_QUARTER_RIGHT;
    return ActionType::ROTATE_QUARTER_LEFT;
}

ActionType Chased::decideNextAction(GameBoard& board, const Tank& self, const Tank& opponent) {
    // 1. Avoid danger if necessary
    if (isDangerAhead(self, board)) {
        return rotateToward(self, opponent);  // Or choose another evasive action
    }

    // 2. If facing opponent and aligned, shoot!
    if ((isAlignedHorizontally(self, opponent) || isAlignedVertically(self, opponent)) &&
        isFacingOpponent(self, opponent)) {
        return ActionType::SHOOT;
    }

    // 3. Not facing opponent? Rotate toward them
    if (!isFacingOpponent(self, opponent)) {
        return rotateToward(self, opponent);
    }

    // 4. Default action: move toward the opponent
    std::pair<int, int> next_cell = {(directionToVector(self.getCanonDirection())).first + self.getX(),
                                (directionToVector(self.getCanonDirection())).second + self.getY()};
    if(board.getCell(next_cell.first,next_cell.second)->getCellType() == CellType::WALL) {
        return ActionType::ROTATE_EIGHTH_LEFT; // Or some other action
    }
    if (board.getCell(next_cell.first, next_cell.second)->getCellType() == CellType::MINE) {
        return ActionType::ROTATE_EIGHTH_LEFT; // Or some other action
    }
    return ActionType::MOVE_FORWARD;
}
