// TankAlgorithm.h
#ifndef TANKALGORITHM_H
#define TANKALGORITHM_H
#include "ActionRequest.h"
#include "BattleInfo.h"
#include <memory>
#include "../Tank.h"
#include "../GameBoard.h"

class TankAlgorithm {
private:

protected:
    std::shared_ptr<Tank> my_tank; // Pointer to the tank controlled by this algorithm
    std::shared_ptr<GameBoard> game_board; // Pointer to the game board
    std::vector<ActionRequest> my_future_moves; // Future moves for the tank
    bool have_battle_info = false;
    std::vector<std::pair<size_t, size_t>> opponents;
public:
    TankAlgorithm(std::shared_ptr<Tank> tank) : my_tank(tank) {};
    virtual ~TankAlgorithm() {} // Virtual destructor for proper cleanup of derived classes
    virtual ActionRequest getAction() = 0;
    virtual void updateBattleInfo(BattleInfo& battleInfo) = 0;
    bool isDangerAhead();
    int isAlignedHorizontally();
    int isAlignedVertically();
    int isFacingOpponent();
    std::shared_ptr<Tank> getTank() const { return my_tank; }
};

#endif //TANKALGORITHM_H


