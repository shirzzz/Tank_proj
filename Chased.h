#ifndef CHASED_H
#define CHASED_H
#include "Tank.h"
#include "GameBoard.h"
#include "DirectionUtils.h"
#include "common/ActionRequest.h"
#include "common/TankAlgorithm.h"

class Chased : public TankAlgorithm {
private:
    //Class methods which are only used in this class
    int player_index; // ADDED: Store player index
    int tank_index;   // ADDED: Store tank index
    
    ActionRequest rotateToward(std::pair<size_t, size_t> opponent);
    // Function to check if the tank is in danger from a shell
    bool isDangerAhead();
    // Function to check if the tank is in danger
    bool isInDanger(const Tank& tank, const GameBoard& board);
    // Main decision function of the Chased tank
    ActionRequest decideNextAction();
    std::vector<ActionRequest> getFutureMoves() const {
        return my_future_moves;
    }
    void setFutureMoves();

public:
    // EXISTING constructor (keep this):
    Chased(std::shared_ptr<Tank> tank) : TankAlgorithm(tank) {};
    
    // ADDED: Constructor that matches your factory:
    Chased(int player_index, int tank_index) : TankAlgorithm(nullptr), 
        player_index(player_index), tank_index(tank_index) {}
    
    ~Chased() = default;
    
    ActionRequest getAction() override;
    
    void updateBattleInfo(BattleInfo& battleInfo) override {
        TankAlgorithm::updateBattleInfo(battleInfo);
    }
};

#endif //CHASED_H