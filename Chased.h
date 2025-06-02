#ifndef CHASED_H
#define CHASED_H
#include "Tank.h"
#include "GameBoard.h"
#include "DirectionUtils.h"
#include "ActionRequest.h"
class Chased : public TankAlgorithm {
private:
    
    ActionRequest rotateToward(std::pair<size_t, size_t> opponent);


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
