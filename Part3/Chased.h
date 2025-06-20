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
    std::pair<int, int> currect_position_of_trail; 
    CanonDirection current_direction; // Current direction of the tank
    std::unique_ptr<TankAlgorithm> clone() const override {
        return std::make_unique<Chased>(*this);
    }
    ActionRequest rotateToward(std::pair<size_t, size_t> opponent);
    int canKillOpponent(std::pair<int, int> pos);
    ActionRequest decideNextAction();
    ActionRequest rotateTowardAndUpdate(int index);
    ActionRequest moveTowardOpponent();
    void rotateCanonRight45();
    void rotateCanonLeft45();
    void rotateCanonRight90();
    void rotateCanonLeft90();
    std::vector<ActionRequest> getFutureMoves() const {
        return my_future_moves;
    }
    void setFutureMoves();

public:
    // EXISTING constructor (keep this):
    Chased() : TankAlgorithm() {};
    
    // ADDED: Constructor that matches your factory:
    Chased(int player_index, int tank_index) : TankAlgorithm(player_index, tank_index){}
    
    ~Chased() = default;
    
    ActionRequest getAction() override;
    
    void updateBattleInfo(BattleInfo& battleInfo) override {
        TankAlgorithm::updateBattleInfo(battleInfo);
    }
};

#endif //CHASED_H