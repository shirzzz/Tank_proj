// TankAlgorithm.h
#ifndef TANKALGORITHM_H
#define TANKALGORITHM_H
#include "../common/ActionRequest.h"
#include "../common/BattleInfo.h"
#include <memory>
#include "../UserCommon/Tank.h"
#include "../Algorithm/GameBoard.h"
#include "../DestructionCause.h"

using Algorithm_211466123_212399455::GameBoard;

class TankAlgorithm {
private:

protected:
    int player_index; // ADDED: Store player index
    int tank_index;   // ADDED: Store tank index
    std::shared_ptr<Tank> my_tank; 
    int width = 0; // Width of the game board
    int height = 0; // Height of the game board
    std::shared_ptr<GameBoard> game_board; // Pointer to the game board
    std::vector<ActionRequest> my_future_moves; // Future moves for the tank
    bool have_battle_info = false;
    std::vector<std::pair<size_t, size_t>> opponents;
    std::vector<std::shared_ptr<Tank>> my_tanks;
    TankAlgorithm() {};
    TankAlgorithm(int player_index, int tank_index)
        : player_index(player_index), tank_index(tank_index) {
        // Initialize the game board and other members if needed
        game_board = nullptr; // Initially no game board
    }
public:

    virtual void updateBattleInfo(BattleInfo& battleInfo) = 0;
    int isFacingOpponent();
    int isFacingOpponent(std::pair<size_t, size_t> position, CanonDirection direction) const;
    int getPlayerIndex() const {
        return player_index;
    }
    int getIndexTank() const {
        return tank_index;
    }
    virtual ~TankAlgorithm() {} // Virtual destructor for proper cleanup of derived classes
    virtual std::unique_ptr<TankAlgorithm> clone() const = 0;
    virtual ActionRequest getAction() = 0;
    void setHaveBattleInfo(bool have_info) {
        have_battle_info = have_info;
    }
};

#endif //TANKALGORITHM_H


