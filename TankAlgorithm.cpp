#include "common/TankAlgorithm.h"
#include "common/ActionRequest.h"
#include "GameBoard.h"
#include "MyBattleInfo.h"
#include "DirectionUtils.h"  

// Check if any shell will move into the tile in front of the tank

//Itai do we need this function?

// bool TankAlgorithm::isDangerAhead() {
//     std::pair<int, int> dir = directionToVector(my_tank.get()->getCanonDirection());  // Itai look at this line I used canon_direction instead of my_tank_ref.getCanonDirection()
//     int dx = dir.first;
//     int dy = dir.second;
//     int fx = my_tank.get()->getX() + dx;  //  Use my_tank_ref instead of tank
//     int fy = my_tank.get()->getY()+ dy;  //  Use my_tank_ref instead of tank
//     //  Use game_board instead of board
//     for (const Shell& shell : game_board->getShells()) {
//         int sx = shell.getX();
//         int sy = shell.getY();
//         auto shell_dir = directionToVector(shell.getDirection());  // Renamed dir to shell_dir to avoid conflict
//         int sdx = shell_dir.first;
//         int sdy = shell_dir.second;
//         for (int step = 0; step < 5; ++step) {
//             if (sx == fx && sy == fy) return true;
//             sx += sdx;
//             sy += sdy;
//             // Use game_board instead of board
//             if (sx < 0 || sx >= static_cast<int>(game_board->getWidth()) || 
//                 sy < 0 || sy >= static_cast<int>(game_board->getHeight()))
//                 break;
//         }
//     }
//     return false;
// }




// check if cannon direction matches direction to opponent
int TankAlgorithm::isFacingOpponent() {  
    int i = 0;
    for(std::pair<size_t, size_t> opponent : opponents) {
        int dx = static_cast<int>(opponent.first) - my_tank.get()->getX();   
        int dy = static_cast<int>(opponent.second) - my_tank.get()->getY(); 
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
        game_board = std::shared_ptr<GameBoard>(myInfo.gameBoard, [](GameBoard*){
            // Don't delete - the original MyBattleInfo owns this pointer
        });
        height = myInfo.gameBoard->getHeight();  
        width = myInfo.gameBoard->getWidth();    
    }
    
    //2. Save my tank
    if (!myInfo.myTanks.empty()) {
        my_tank = myInfo.myTanks[tank_index];  
    } else {
        throw std::runtime_error("No tanks available in MyBattleInfo");
    }
    //3. Save my tanks
    my_tanks = myInfo.myTanks;  

    // 4. Save enemy locations directly
    opponents = myInfo.knownEnemyLocations;  

    // 5. Set flag that we have battle info
    have_battle_info = true;
}

    int TankAlgorithm::isFacingOpponent(std::pair<size_t, size_t> position, CanonDirection direction) const {
        int i = 0;
            for(std::pair<size_t, size_t> opponent : opponents) {
                int dx = static_cast<int>(opponent.first) - position.first;   
                int dy = static_cast<int>(opponent.second) - position.second; 
                if (dx == 0 && dy == 0) {
                    i++;
                    continue; // Same position, check next opponent
                }
                CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
                if (direction == dirToOpponent) { 
                    
                    return i; // Facing opponent
                }
                i++;
            }
            return -1; // Not facing any opponent
        }