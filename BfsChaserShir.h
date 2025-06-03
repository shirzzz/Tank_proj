
#ifndef BFSCHASERSHIR_H
#define BFSCHASERSHIR_H
#pragma once
#include <memory>
#include "common/TankAlgorithm.h"  
#include "common/ActionRequest.h"
#include "GameBoard.h"

class BfsChaserShir : public TankAlgorithm {
private:
//Class methods which are only used in this class
    std::vector<std::vector<int>> getGraphOutOfBoard(const std::shared_ptr<GameBoard> board);
    std::vector<int> getFutureMovesBfs(const std::vector<std::vector<int>>& graph, int start_node);
    std::vector<ActionRequest> setFutureMoves(const std::vector<int>& path, int height);
    int canKillOpponent(int spot);
    std::pair<int, int> fromIndextoPos(int index, int height) const;
    bool isChased(const Tank& self, const std::shared_ptr<GameBoard> board);

public:
    BfsChaserShir(std::shared_ptr<Tank> tank): my_tank(tank){};
    ~BfsChaserShir() override = default;
    ActionRequest getAction() override;
   
};



#endif //BFSCHASERSHIR_H
