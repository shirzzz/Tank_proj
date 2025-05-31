
#ifndef BFSCHASERSHIR_H
#define BFSCHASERSHIR_H
#pragma once
#include <memory>
#include "common/TankAlgorithm.h"  
#include "common/ActionRequest.h"
#include "GameBoard.h"

class BfsChaserShir : public TankAlgorithm {
private:
    std::vector<ActionRequest> my_future_moves;
    std::shared_ptr<GameBoard> game_board;
    vector<std::shared_ptr<Tank>> opponents; // List of opponent tanks
    std::shared_ptr<Tank> my_tank;
public:
    BfsChaserShir() = default;
    ~BfsChaserShir() override = default;
    ActionRequest getAction() override;
    std::vector<std::vector<int>> getGraphOutOfBoard(const std::shared_ptr<GameBoard> board);
    std::vector<int> getFutureMovesBfs(const std::vector<std::vector<int>>& graph, int start_node);
    std::vector<ActionRequest> setFutureMoves(const std::vector<int>& path, int height);
    int canKillOpponent(int spot);
    std::pair<int, int> fromIndextoPos(int index, int height) const;
    bool isChased(const Tank& self, const std::shared_ptr<GameBoard> board);

};



#endif //BFSCHASERSHIR_H
