// #ifndef PLAYER1ALGORITHM_H
// #define PLAYER1ALGORITHM_H

// #include "TankAlgorithm.h"
// #include "ActionRequest.h"
// #include "BattleInfo.h"
// #include "Tank.h"
// #include "GameBoard.h"
// #include "Player1BattleInfo.h"
// #include "CanonDirection.h"
// #include <memory>
// #include <vector>
// #include <queue>
// #include <unordered_map>
// #include <unordered_set>

// class Player1Algorithm : public TankAlgorithm {
// private:
//     std::vector<ActionRequest> my_future_moves;
//     std::vector<Tank> opponent_tanks;
//     std::shared_ptr<GameBoard> shared_board;
    
// public:
//     Player1Algorithm() = default;
//     ~Player1Algorithm() override = default;
    
//     // Required TankAlgorithm interface methods
//     ActionRequest getAction() override;
//     void updateBattleInfo(BattleInfo& battleInfo) override;
    
//     // Graph and pathfinding methods
//     std::vector<std::vector<int>> getGraphOutOfBoard(const std::shared_ptr<GameBoard> board);
//     std::vector<int> getFutureMovesBFS(Tank& start_tank, std::vector<std::vector<int>> graph, std::vector<Tank> opponent_tanks);
//     std::vector<ActionRequest> getFutureMoves(std::vector<int> path, Tank& tank, std::vector<Tank> opponent_tanks, int height);
    
//     // Combat and positioning methods
//     bool isFacingOpponent(const Tank& self, const Tank& opponent);
//     bool isTankFacingAnyOpponent(const Tank& self, std::vector<Tank> opponent_tanks);
//     int canKillOpponent(int current_tank, std::vector<Tank> opponent_tanks);
// };

// #endif // PLAYER1ALGORITHM_H