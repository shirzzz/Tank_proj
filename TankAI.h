#ifndef TANK_AI_H
#define TANK_AI_H

#include "GameBoard.h"
#include "Tank.h"
#include "ActionType.h"

class TankAI {
public:
    virtual ~TankAI() = default;

    // This is the main function each AI must implement
    virtual ActionType decideNextAction(const GameBoard& board, const Tank& self, const Tank& opponent) = 0;
};

#endif // TANK_AI_H