#ifndef BFSCHASERSHIR_Hs
#define BFSCHASERSHIR_H
#pragma once
#include <memory>
#include "../common/TankAlgorithm.h"  
#include "common/ActionRequest.h"
#include "GameBoard.h"
namespace Algorithm_211466123_212399455{
class BfsChaserShir : public TankAlgorithm {
private:
    CanonDirection current_direction;
    //Class methods which are only used in this class
    int canKillOpponent(int spot);
    std::vector<std::vector<int>> getGraphOutOfBoard();
    std::vector<int> getFutureMovesBfs(const std::vector<std::vector<int>>& graph, int start_node);
    void setFutureMoves(const std::vector<int>& path, int height, int start);
    void handleEmptyPath(int start, int height);
    bool tryToShootOpponent(int indexPos, int height);
    void rotateTowardsAndMoveForward(int x1, int y1, int x2, int y2);
    void rotateCanonTowards(CanonDirection target_direction);
    std::pair<int, int> fromIndextoPos(int index, int height);
    std::vector<int> opponentsSpots();
    void rotateCanonRight45();
    void rotateCanonLeft45();
    void rotateCanonRight90();
    void rotateCanonLeft90();
    //checking if the tank is shooting its teammates
    bool amIshootingMyTeammates() const;
    //running away from shells
    bool isShellThreateningMe(const Shell& shell) const;
    std::vector<Shell> shouldRunAwayFromShells() const;
    bool shouldIShootShell(const Shell& shell) const;
    void setSafeEscapeMoves(std::vector<Shell>& shells);
    CanonDirection findEscapeDirection(const std::vector<Shell>& shells, int radius) const;


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
}