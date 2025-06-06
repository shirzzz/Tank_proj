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
    
    std::pair<int, int> fromIndextoPos(int index, int height);
    bool isChased(const Tank& self, const std::shared_ptr<GameBoard> board);
    std::vector<int> opponentsSpots();

public:
    // EXISTING constructor (keep this):
    // BfsChaserShir(std::shared_ptr<Tank> tank): TankAlgorithm(tank){};
    std::unique_ptr<TankAlgorithm> clone() const override {
        return std::make_unique<BfsChaserShir>(*this);
    }
    // FIXED: Constructor that matches your factory - pass nullptr to base class:
    BfsChaserShir(int player_index, int tank_index) : TankAlgorithm(player_index, tank_index){}
    
    ~BfsChaserShir() override = default;
    
    ActionRequest getAction() override;
    
    void updateBattleInfo(BattleInfo& battleInfo) override {
        TankAlgorithm::updateBattleInfo(battleInfo);
    }
};

#endif //BFSCHASERSHIR_H