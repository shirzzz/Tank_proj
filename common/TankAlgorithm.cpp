#include "TankAlgorithm.h"
#include "Chased.h"
#include "ActionRequest.h"
#include "GameBoard.h"

// Check if any shell will move into the tile in front of the tank
bool TankAlgorithm::isDangerAhead() {
    Tank& my_tank = *this->my_tank.get();
    const GameBoard& board = *this->game_board.get();
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
int TankAlgorithm::isAlignedHorizontally() {
    int i = 0;
    //returns which opponent is aligned with self
    for(const auto& opponent : opponents) {
        if (my_tank.get().getY() == opponent.get().getY()) {
            return i;
        }
        i++;
    }
    return -1; // No opponent aligned horizontally
}

int TankAlgorithm::isAlignedVertically() {
    int i = 0;
    for(const auto& opponent : opponents) {
        if (my_tank.get().getX() == opponent.get().getX()) {
            return i;
        }
        i++;
    }
    return -1; // No opponent aligned vertically
}

// Helper: check if cannon direction matches direction to opponent
intTankAlgorithm::isFacingOpponent() {
    Tank& self = *my_tank.get();
    int i = 0;
    for(const auto& opponent : opponents) {
        if (opponent == nullptr) continue; // Skip null opponents
        int dx = opponent.getX() - self.getX();
        int dy = opponent.getY() - self.getY();
        if (dx == 0 && dy == 0) return false; // Same position (somehow?)
        CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
        if (self.getCanonDirection() == dirToOpponent) {
            return i; // Facing opponent
        }
        i++;
    }
    return -1; // Not facing any opponent
}