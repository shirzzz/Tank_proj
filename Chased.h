#ifndef CHASED_H
#define CHASED_H
#include "Tank.h"
#include "GameBoard.h"
#include "DirectionUtils.h"
#include "ActionRequest.h"
class Chased : public TankAlgorithm {
private:
    std::shared_ptr<Tank> my_tank;
    vector<ActionRequest> my_future_moves;
//Hi Itai these are the things I need from battle info
    vector<std:shared_ptr<Tank>> opponents; // List of opponent tanks
    vector<GameBoard> game_board; // Game board representation


    // Helper function to check if the tank is aligned with the opponent
    bool isAlignedHorizontally();
    bool isAlignedVertically();
    // Helper function to check if the tank's cannon direction matches the direction to the opponent
    bool isFacingOpponent();
    // Helper function to compute the best rotation toward the opponent's position
    ActionRequest rotateToward(std::shared_ptr<Tank> opponent);


public:
    Chased() = default;
    ~Chased() = default;

    // Function to check if the tank is in danger from a shell
    bool isDangerAhead();
    // Function to check if the tank is in danger
    bool isInDanger(const Tank& tank, const GameBoard& board);
    // Main decision function of the Chased tank
    ActionRequest decideNextAction(GameBoard& board, const Tank& self, const Tank& opponent);
    ActionRequest getAction() override;
    vector<ActionRequest> getFutureMoves() const {
        return my_future_moves;
    }
    void setFutureMoves();


};



#endif //CHASED_H
