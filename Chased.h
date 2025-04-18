#ifndef CHASED_H
#define CHASED_H
#include "Tank.h"
#include "GameBoard.h"
#include "DirectionUtils.h"
#include "ActionType.h"
class Chased {
public:
    Chased() = default;
    ~Chased() = default;

    // Function to check if the tank is in danger from a shell
    bool isDangerAhead(const Tank& tank, const GameBoard& board);
    // Function to check if the tank is in danger
    bool isInDanger(const Tank& tank, const GameBoard& board);
    // Main decision function of the Chased tank
    ActionType decideNextAction(GameBoard& board, const Tank& self, const Tank& opponent);

};



#endif //CHASED_H
