#include "common/TankAlgorithm.h"
#include "common/ActionRequest.h"
#include "GameBoard.h"
#include "MyBattleInfo.h"
#include "DirectionUtils.h"  // ADDED: Need this for directionToVector function

// Check if any shell will move into the tile in front of the tank
bool TankAlgorithm::isDangerAhead() {
    //Tank& my_tank_ref = *this->my_tank.get();  // FIXED: Renamed variable to avoid confusion
    std::pair<int, int> dir = directionToVector(my_tank.get()->getCanonDirection());  // Itai look at this line I used canon_direction instead of my_tank_ref.getCanonDirection()
    int dx = dir.first;
    int dy = dir.second;
    int fx = my_tank.get()->getX() + dx;  // FIXED: Use my_tank_ref instead of tank
    int fy = my_tank.get()->getY()+ dy;  // FIXED: Use my_tank_ref instead of tank

    // FIXED: Use game_board instead of board
    for (const Shell& shell : game_board->getShells()) {
        int sx = shell.getX();
        int sy = shell.getY();
        auto shell_dir = directionToVector(shell.getDirection());  // FIXED: Renamed dir to shell_dir to avoid conflict
        int sdx = shell_dir.first;
        int sdy = shell_dir.second;

        for (int step = 0; step < 5; ++step) {
            if (sx == fx && sy == fy) return true;

            sx += sdx;
            sy += sdy;
            // FIXED: Use game_board instead of board
            if (sx < 0 || sx >= static_cast<int>(game_board->getWidth()) || 
                sy < 0 || sy >= static_cast<int>(game_board->getHeight()))
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
        // FIXED: Use my_tank.get()->getY() instead of my_tank.get().getY()
        if (my_tank.get()->getY() == static_cast<int>(opponent.second)) {  // //Itai I deleted the cast to int here because my_tank.get()->getY() is already an int
            return i;
        }
        i++;
    }
    return -1; // No opponent aligned horizontally
}

int TankAlgorithm::isAlignedVertically() {
    int i = 0;
    for(std::pair<size_t, size_t> opponent : opponents) {
        // FIXED: Use my_tank.get()->getX() instead of my_tank.get().getX()
        if (my_tank.get()->getX() == static_cast<int>(opponent.first)) {  // Itai I deleted the cast to int here because my_tank.get()->getX() is already an int
            return i;
        }
        i++;
    }
    return -1; // No opponent aligned vertically
}

// Helper: check if cannon direction matches direction to opponent
int TankAlgorithm::isFacingOpponent() {  // FIXED: Added missing return type 'int'
    //Tank& self = *my_tank.get();
    int i = 0;
    for(std::pair<size_t, size_t> opponent : opponents) {
        // FIXED: Remove nullptr check - size_t pairs can't be nullptr
        int dx = static_cast<int>(opponent.first) - my_tank.get()->getX();   // FIXED: Cast size_t to int
        int dy = static_cast<int>(opponent.second) - my_tank.get()->getY(); // FIXED: Cast size_t to int
        if (dx == 0 && dy == 0) {
            i++;
            continue; // Same position, check next opponent
        }
        CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
        if (my_tank.get()->getCanonDirection() == dirToOpponent) { //Itai here I used canon_direction instead of my_tank_ref.getCanonDirection()
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
        // FIXED: Use shared_ptr properly - don't create custom deleter that does nothing
        game_board = std::shared_ptr<GameBoard>(myInfo.gameBoard, [](GameBoard*){
            // Don't delete - the original MyBattleInfo owns this pointer
        });
        height = myInfo.gameBoard->getHeight();  // FIXED: Use game_board to get height
        width = myInfo.gameBoard->getWidth();    // FIXED: Use game_board to get width
    }
    
    //2. Save my tank
    if (!myInfo.myTanks.empty()) {
        my_tank = myInfo.myTanks[tank_index];  // FIXED: Use tank_index to access the correct tank
    } else {
        throw std::runtime_error("No tanks available in MyBattleInfo");
    }
    //3. Save my tanks
    my_tanks = myInfo.myTanks;  // FIXED: Save all tanks for this player

    // 4. Save enemy locations directly
    opponents = myInfo.knownEnemyLocations;  

    // 5. Set flag that we have battle info
    have_battle_info = true;
}