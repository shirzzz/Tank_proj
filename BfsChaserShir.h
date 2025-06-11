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
    int canKillOpponent(int spot);
    std::vector<std::vector<int>> getGraphOutOfBoard();
    std::vector<int> getFutureMovesBfs(const std::vector<std::vector<int>>& graph, int start_node);
    void setFutureMoves(const std::vector<int>& path, int height, int start);
    
    std::pair<int, int> fromIndextoPos(int index, int height);
    bool isChased(const Tank& self, const std::shared_ptr<GameBoard> board);
    std::vector<int> opponentsSpots();

public:
    std::unique_ptr<TankAlgorithm> clone() const override {
        return std::make_unique<BfsChaserShir>(*this);
    }
    BfsChaserShir(int player_index, int tank_index) : TankAlgorithm(player_index, tank_index){}
    
    ~BfsChaserShir() override = default;
    
    ActionRequest getAction() override;
    
    void updateBattleInfo(BattleInfo& battleInfo) override {
        TankAlgorithm::updateBattleInfo(battleInfo);
    }
};

#endif //BFSCHASERSHIR_H