#ifndef PLAYER2ALGORITHM_H
#define PLAYER2ALGORITHM_H

#include "TankAlgorithm.h"
#include "ActionRequest.h"
#include "BattleInfo.h"
#include "Tank.h" 
#include "GameBoard.h"
#include "Player2BattleInfo.h"
#include "Chased.h"
#include <memory>
#include <vector>

class Player2Algorithm : public TankAlgorithm {
private:
    std::vector<Tank> opponent_tanks;
    std::shared_ptr<GameBoard> board;
    Tank* current_tank = nullptr;
    
public:
    Player2Algorithm() = default;
    ~Player2Algorithm() override = default;
    
    // Required TankAlgorithm interface methods
    ActionRequest getAction() override;  // NOT "Player2Algorithm::getAction()"
    void updateBattleInfo(BattleInfo& battleInfo) override;
    
    // Setter for current tank
    void setCurrentTank(Tank* tank) { current_tank = tank; }
    Tank* getCurrentTank() const { return current_tank; }
};

#endif // PLAYER2ALGORITHM_H