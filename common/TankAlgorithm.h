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
    int player_index; // ADDED: Store player index
    int tank_index;   // ADDED: Store tank index
    std::pair<size_t, size_t> my_tank; // Position of the tank
    //Itai I need to have the canon direction of my tank and also the positions of this player's tanks
    CanonDirection canon_direction; // Direction of the tank's cannon
    std::shared_ptr<GameBoard> game_board; // Pointer to the game board
    std::vector<ActionRequest> my_future_moves; // Future moves for the tank
    bool have_battle_info = false;
    std::vector<std::pair<size_t, size_t>> opponents;
    std::vector<std::pair<size_t, size_t>> my_tanks_positions; // Positions of my tanks
public:
    TankAlgorithm() {};
    TankAlgorithm(int player_index, int tank_index)
        : player_index(player_index), tank_index(tank_index) {
        // Initialize the game board and other members if needed
        game_board = nullptr; // Initially no game board
    }
    virtual ~TankAlgorithm() {} // Virtual destructor for proper cleanup of derived classes
    virtual std::unique_ptr<TankAlgorithm> clone() const = 0;
    virtual ActionRequest getAction() = 0;
    virtual void updateBattleInfo(BattleInfo& battleInfo) = 0;
    bool isDangerAhead();
    int isAlignedHorizontally();
    int isAlignedVertically();
    int isFacingOpponent();
    
    int getPlayerIndex() const {
        return player_index;
    }
    int getIndexTank() const {
        return tank_index;
    }
};

#endif //TANKALGORITHM_H


