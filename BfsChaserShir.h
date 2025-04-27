
#ifndef BFSCHASERSHIR_H
#define BFSCHASERSHIR_H
#pragma once
#include <memory>

#include "GameBoard.h"

class BfsChaserShir {
private:
    std::vector<ActionType> my_future_moves;
public:
    std::vector<int> getFutureMovesBfs(std::vector<std::vector<int>> graph, int start, int end);
    std::vector<ActionType> getFutureMoves(std::vector<int> path, std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2, int height);
    ActionType getNextMove(std::shared_ptr<GameBoard> board, std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2);
    std::vector<std::vector<int>> getGraphOutOfBoard(const std::shared_ptr<GameBoard> board);
    bool isFacingOpponent(const Tank& self, const Tank& opponent);
    bool isChased(const Tank& self, const std::shared_ptr<GameBoard> board);
};



#endif //BFSCHASERSHIR_H
