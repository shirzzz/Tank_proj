#ifndef PLAYER2ALGORITHM_H
#define PLAYER2ALGORITHM_H
#pragma once
#include "TankAlgorithm.h"
#include "ActionRequest.h"
#include "Tank.h"
#include "GameBoard.h"
#include "Player.h"
#include "Player2.h"
#include "Chased.h"
class Player2Algorithm : public TankAlgorithm {
public:
    Player2Algorithm() = default;
    ~Player2Algorithm() = default;
    ActionRequest Player2Algorithm::decideNextAction(GameBoard& board, Tank& tank, std::vector<Tank>& opponent_tanks);

};



#endif //PLAYER2ALGORITHM_H
