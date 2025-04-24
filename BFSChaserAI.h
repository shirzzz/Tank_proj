#ifndef BFS_CHASER_AI_H
#define BFS_CHASER_AI_H

#include "TankAI.h"

class BFSChaserAI : public TankAI {
    private:
    std::pair<int, int> findNextStepTowardOpponent(const GameBoard& board, const Tank& self, const Tank& opponent);
    std::vector<ActionType> my_future_moves(const Tank& tank, const GameBoard& board);
    public:
        ActionType decideNextAction(const GameBoard& board, const Tank& self, const Tank& opponent) override;
        bool isDangerAhead(const Tank& tank, const GameBoard& board);
        std::vector<ActionType> getFutureMoves(const Tank& tank, const GameBoard& board);
        ActionType getNextMove();


    };
#endif // BFS_CHASER_AI_H


