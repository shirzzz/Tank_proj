
#ifndef PLAYER1ALGORITHM_H
#define PLAYER1ALGORITHM_H
#pragma once
#include "TankAlgorithm.h"
#include "ActionRequest.h"
#include "Tank.h"
#include "GameBoard.h"
#include "Player.h"
#include "Player2.h"
#include <memory>

class Player1Algorithm : public TankAlgorithm {
private:
    std::vector<ActionRequest> my_future_moves;
public:
    ~Player1Algorithm() override = default;
    void updateBattleInfo(BattleInfo& battleInfo) override;
    CanonDirection rotateDirectionLeft(CanonDirection dir);
    CanonDirection rotateDirectionRight(CanonDirection dir);
    std::vector<std::vector<int>> getGraphOutOfBoard(const std::shared_ptr<GameBoard> board);
    std::unordered_map<int, std::vector<int>> getFutureMovesMultiSourcesBFS(std::vector<std::vector<int>> graph, std::vector<Tank> opponent_tanks);
    std::vector<std::vector<ActionRequest>> getFutureMoves(std::unordered_map<int, std::vector<int>> path, Player1& player1, std::vector<Tank> opponent_tanks, int height);
    std::vector<ActionRequest> getAction() override;
    int isChased(const Player1& self, const std::shared_ptr<GameBoard> board);
    bool isFacingOpponent(const Tank& self, const Tank& opponent);
    bool isFacingAnyOpponent(Player1 &player1, std::vector<Tank> opponent_tanks);
    int canKillOpponent(int current_tank, std::vector<Tank> opponent_tanks);
    bool isTankFacingAnyOpponent(const Tank& self, std::vector<Tank>& opponent_tanks);
};



#endif //BFSCHASERSHIR_H
