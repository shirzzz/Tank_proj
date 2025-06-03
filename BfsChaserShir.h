
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
    int height = 0; // Height of the game board
    int width = 0; // Width of the game board
    int canKillOpponent(int spot);
    std::vector<std::vector<int>> getGraphOutOfBoard();
    std::vector<int> getFutureMovesBfs(const std::vector<std::vector<int>>& graph, int start_node);
    void setFutureMoves(const std::vector<int>& path, int height);
    
    std::pair<int, int> fromIndextoPos(int index, int height) ;
    bool isChased(const Tank& self, const std::shared_ptr<GameBoard> board);
    std::vector<int> opponentsSpots();

public:
    BfsChaserShir(std::shared_ptr<Tank> tank): TankAlgorithm(tank){};
    ~BfsChaserShir() override = default;
    ActionRequest getAction() override;
   
};



#endif //BFSCHASERSHIR_H
