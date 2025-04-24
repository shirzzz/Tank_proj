#ifndef BFS_CHASER_AI_H
#define BFS_CHASER_AI_H

#include "TankAI.h"

class BFSChaserAI : public TankAI {
    public:
        ActionType decideNextAction(const GameBoard& board, const Tank& self, const Tank& opponent) override;
        bool isDangerAhead(const Tank& tank, const GameBoard& board);
    private:
        std::pair<int, int> findNextStepTowardOpponent(const GameBoard& board, const Tank& self, const Tank& opponent);
    };
#endif // BFS_CHASER_AI_H


