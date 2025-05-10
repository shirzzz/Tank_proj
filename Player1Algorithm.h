
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
    std::unordered_map<int, std::vector<int>> getFutureMovesMultiSourcesBFS(std::vector<std::vector<int>> graph, Player2& opponent);
    std::vector<std::vector<ActionRequest>> getFutureMoves(std::unordered_map<int, std::vector<int>> path, Player1& player1, Player2& player2, int height);
    std::vector<ActionRequest> getNextMove(std::shared_ptr<GameBoard> shared_board, std::shared_ptr<Tank> tank1, Player2& player2);
    int isChased(const Player1& self, const std::shared_ptr<GameBoard> board);
    bool isFacingOpponent(const Tank& self, const Tank& opponent);
    bool isFacingAnyOpponent(Player1 &player1, Player2 & player2);
    int canKillOpponent(int current_tank, Player2 & player2);
    bool isTankFacingAnyOpponent(const Tank& self, Player2 & opponent);
};



#endif //BFSCHASERSHIR_H
