#include "common/TankAlgorithm.h"
#include "common/ActionRequest.h"
#include "GameBoard.h"
#include "MyBattleInfo.h"
#include "DirectionUtils.h"  
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