#include "TankAlgorithm.h"
#include "ActionRequest.h"
#include "GameBoard.h"
#include "../MyBattleInfo.h"

// Check if any shell will move into the tile in front of the tank
bool TankAlgorithm::isDangerAhead() {
    Tank& my_tank = *this->my_tank.get();  
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
    for(std::pair<size_t, size_t> opponent : opponents) {
        if (my_tank.get().getY() == opponent.second) {
            return i;
        }
        i++;
    }
    return -1; // No opponent aligned horizontally
}

int TankAlgorithm::isAlignedVertically() {
    int i = 0;
    for(std::pair<size_t, size_t> opponent : opponents) {
        if (my_tank.get().getX() == opponent.first) {
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
    for(std::pair<size_t, size_t> opponent : opponents) {
        if (opponent == nullptr) continue; // Skip null opponents
        int dx = opponent.first- self.getX();
        int dy = opponent.second - self.getY();
        if (dx == 0 && dy == 0) return false; // Same position (somehow?)
        CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
        if (self.getCanonDirection() == dirToOpponent) {
            return i; // Facing opponent
        }
        i++;
    }
    return -1; // Not facing any opponent
}
void TankAlgorithm::updateBattleInfo(BattleInfo& info) {
    // Cast to MyBattleInfo
    MyBattleInfo& myInfo = static_cast<MyBattleInfo&>(info);
        
    // 1. Save the game board
    if (myInfo.gameBoard) {
        game_board = std::shared_ptr<GameBoard>(myInfo.gameBoard, [](GameBoard*){});
    }
    
    // 2. Save enemy locations directly
   opponents = myInfo.knownEnemyLocations;  
    // 3. Set flag that we have battle info
    have_battle_info = true;
}