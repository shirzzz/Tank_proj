// #include "Player1Algorithm.h"
// #include "Player1BattleInfo.h"
// #include "DirectionUtils.h"
// #include <queue>
// #include <unordered_map>
// #include <unordered_set>
// #include <algorithm>
// #include <iostream>

// ActionRequest Player1Algorithm::getAction() {
//     if (!my_future_moves.empty()) {
//         ActionRequest next_move = my_future_moves.front();
//         my_future_moves.erase(my_future_moves.begin());
//         return next_move;
//     } else {
//         return ActionRequest::GetBattleInfo;
//     }
// }

// void Player1Algorithm::updateBattleInfo(BattleInfo& battleInfo) {
//     auto& myInfo = dynamic_cast<Player1BattleInfo&>(battleInfo);
//     opponent_tanks = myInfo.opponents;
//     shared_board = myInfo.board;
    
//     // Build moves using your existing logic
//     if (!opponent_tanks.empty() && shared_board) {
//         std::vector<std::vector<int>> graph = getGraphOutOfBoard(shared_board);
//         // For now, use first opponent tank - you can enhance this later
//         Tank dummy_tank(0, 0, '1'); // Placeholder - you'll need actual tank reference
//         std::vector<int> path = getFutureMovesBFS(dummy_tank, graph, opponent_tanks);
//         my_future_moves = getFutureMoves(path, dummy_tank, opponent_tanks, shared_board->getHeight());
//     }
// }

// std::vector<std::vector<int>> Player1Algorithm::getGraphOutOfBoard(const std::shared_ptr<GameBoard> board) {
//     int width = board->getWidth();
//     int height = board->getHeight();
//     std::vector<std::vector<int>> graph(width * height);
    
//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             if (!board->isCellPassable(x, y)) continue;
//             int cell_index = x * height + y;

//             // Add neighbors (your existing logic)
//             if (x > 0 && board->isCellPassable(x-1, y)) {
//                 graph[cell_index].push_back((x-1) * height + y);
//             }
//             if (x < width - 1 && board->isCellPassable(x+1, y)) {
//                 graph[cell_index].push_back((x+1) * height + y);
//             }
//             if (y > 0 && board->isCellPassable(x, y-1)) {
//                 graph[cell_index].push_back(x * height + (y-1));
//             }
//             if (y < height - 1 && board->isCellPassable(x, y+1)) {
//                 graph[cell_index].push_back(x * height + (y+1));
//             }
//         }
//     }
//     return graph;
// }

// std::vector<int> Player1Algorithm::getFutureMovesBFS(Tank& start_tank, std::vector<std::vector<int>> graph, std::vector<Tank> opponent_tanks) {
//     if (opponent_tanks.empty()) return {};
    
//     std::queue<int> q;
//     std::unordered_set<int> visited;
//     std::unordered_map<int, int> parent;
    
//     int height = shared_board ? shared_board->getHeight() : 8; // Default fallback
//     int start_pos = start_tank.getX() * height + start_tank.getY();
//     int target_pos = opponent_tanks[0].getX() * height + opponent_tanks[0].getY();
    
//     q.push(start_pos);
//     visited.insert(start_pos);
    
//     while (!q.empty()) {
//         int current = q.front();
//         q.pop();
        
//         if (current == target_pos) {
//             // Reconstruct path
//             std::vector<int> path;
//             int at = target_pos;
//             while (parent.find(at) != parent.end()) {
//                 path.push_back(at);
//                 at = parent[at];
//             }
//             path.push_back(start_pos);
//             std::reverse(path.begin(), path.end());
//             return path;
//         }
        
//         if (current < static_cast<int>(graph.size())) {
//             for (int neighbor : graph[current]) {
//                 if (visited.find(neighbor) == visited.end()) {
//                     visited.insert(neighbor);
//                     parent[neighbor] = current;
//                     q.push(neighbor);
//                 }
//             }
//         }
//     }
    
//     return {}; // No path found
// }

// std::vector<ActionRequest> Player1Algorithm::getFutureMoves(std::vector<int> path, Tank& tank, std::vector<Tank> opponent_tanks, int height) {
//     std::vector<ActionRequest> moves;
    
//     if (path.empty()) {
//         moves.push_back(ActionRequest::Shoot);
//         return moves;
//     }
    
//     // Convert path positions to movement commands
//     for (size_t i = 1; i < path.size(); ++i) {
//         int x1 = path[i-1] / height;
//         int y1 = path[i-1] % height;
//         int x2 = path[i] / height;
//         int y2 = path[i] % height;
//         int dx = x2 - x1;
//         int dy = y2 - y1;
        
//         // Check if we can shoot the opponent first
//         if (isTankFacingAnyOpponent(tank, opponent_tanks)) {
//             moves.push_back(ActionRequest::Shoot);
//             continue;
//         }
        
//         // Calculate the direction we need to face
//         CanonDirection targetDirection = getDirectionFromDelta(dx, dy);
//         CanonDirection currentDirection = tank.getCanonDirection();
//         int current = static_cast<int>(currentDirection);
//         int target = static_cast<int>(targetDirection);
        
//         // Calculate rotation needed
//         int right_steps = (target - current + 8) % 8;
//         int left_steps = (current - target + 8) % 8;
        
//         // Choose the shorter rotation - just add the ActionRequests
//         if (left_steps <= right_steps) {
//             for (int j = 0; j < left_steps; ++j) {
//                 moves.push_back(ActionRequest::RotateLeft45);
//             }
//         } else {
//             for (int j = 0; j < right_steps; ++j) {
//                 moves.push_back(ActionRequest::RotateRight45);
//             }
//         }
        
//         // Move forward after rotation
//         moves.push_back(ActionRequest::MoveForward);
//     }
    
//     return moves;
// }

// bool Player1Algorithm::isFacingOpponent(const Tank& self, const Tank& opponent) {
//     int dx = opponent.getX() - self.getX();
//     int dy = opponent.getY() - self.getY();
//     if (dx == 0 && dy == 0) return false;
    
//     CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
//     return self.getCanonDirection() == dirToOpponent;
// }

// bool Player1Algorithm::isTankFacingAnyOpponent(const Tank& self, std::vector<Tank> opponent_tanks) {
//     for (const Tank& opponent : opponent_tanks) {
//         if (isFacingOpponent(self, opponent)) {
//             return true;
//         }
//     }
//     return false;
// }

// int Player1Algorithm::canKillOpponent(int current_tank, std::vector<Tank> opponent_tanks) {
//     if (!shared_board) return -1;
    
//     int height = shared_board->getHeight();
//     int x = current_tank / height;
//     int y = current_tank % height;
    
//     for (size_t i = 0; i < opponent_tanks.size(); ++i) {
//         const Tank& opponent = opponent_tanks[i];
//         int dx = opponent.getX() - x;
//         int dy = opponent.getY() - y;
        
//         // Check if opponent is in a straight line (horizontal, vertical, or diagonal)
//         if (dx == 0 || dy == 0 || std::abs(dx) == std::abs(dy)) {
//             // Check if there are no obstacles in the way
//             // This is a simplified check - you can enhance it
//             return static_cast<int>(i);
//         }
//     }
    
//     return -1; // No direct kill opportunity
// }

// // Remove these methods - they're not needed since we don't track direction locally
// // The GameManager handles the actual tank rotation when processing ActionRequests