// TankAlgorithm.h
#ifndef TANKALGORITHM_H
#define TANKALGORITHM_H
#include "ActionRequest.h"
#include "BattleInfo.h"
#include <memory>
class TankAlgorithm {
private:
    std::shared_ptr<Tank> my_tank; // Pointer to the tank controlled by this algorithm
    std::shared_ptr<GameBoard> game_board; // Pointer to the game board
    std::vector<std::shared_ptr<Tank>> opponents; // List of opponent tanks
public:
    virtual ~TankAlgorithm() {} // Virtual destructor for proper cleanup of derived classes
    virtual ActionRequest getAction() = 0;
    virtual void updateBattleInfo(BattleInfo& battleInfo) = 0;
    bool Chased::isDangerAhead();
    int isAlignedHorizontally();
    int isAlignedVertically();
    int isFacingOpponent();
};

#endif //TANKALGORITHM_H


