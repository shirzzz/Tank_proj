#include "BfsChaserShir.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include "GameBoard.h"
#include "DirectionUtils.h"

CanonDirection rotateDirectionLeft(CanonDirection dir) {
    return static_cast<CanonDirection>((static_cast<int>(dir) + 7) % 8);
}

CanonDirection rotateDirectionRight(CanonDirection dir) {
    return static_cast<CanonDirection>((static_cast<int>(dir) + 1) % 8);
}

std::vector<int> Player1Algorithm::getFutureMovesBFS(Tank &start_tank, std::vector<std::vector<int>> graph, vector<Tank> opponent_tanks) {
    std::queue<int> q;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;
    std::vector<int> path;
    std::vector<Tank> tanks = self.getTanks();

    int start = start_tank.getX() * graph[0].size() + start_tank.getY();
    if (start < 0 || static_cast<size_t>(start) >= graph.size()) {
        return {};
    }
    q.push(start);
    visited.insert(start);
    int found_pos = -1;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (canKillOpponent(current, opponent_tanks) != -1) {
            int source = source_map[current];
            std::vector<int> path;
            int at = current;
            while(parent.find(at) != parent.end()) {
                path.push_back(at);
                at = parent[at];
            }
            path.push_back(at);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }
    return {};
}

std::vector<ActionRequest> Player1Algorithm::getFutureMoves(std::vector<int> path, Tank& tank, std::vector<Tank> opponent_tanks, int height) {
   std::vector<ActionRequest> moves;
    if (path.empty()) {
        moves.push_back(ActionRequest::SHOOT);
        return moves;
    }
    int start_node = tank.getX() * height + tank.getY();

    CanonDirection canon_direction = tank->getCanonDirection();

    for (size_t i = 1; i < path.size(); ++i) {
        int x1 = path[i-1] / height;
        int y1 = path[i-1] % height;
        int x2 = path[i] / height;
        int y2 = path[i] % height;
        int dx = x2 - x1;
        int dy = y2 - y1;
    }
    if (isTankFacingAnyOpponent(tank, opponent_tanks)) {
        moves.push_back(ActionRequest::SHOOT);
        continue;
    }
    //there is a possibility to kill the opponent
    int can_kill = canKillOpponent(start_node, opponent_tanks);
     if(can_kill != -1) {
        const Tank& opponent_tank = opponent_tanks[can_kill];
        int opp_x = opponent_tank.getX();
        int opp_y = opponent_tank.getY();
        int dx = opp_x - tank.getX();
        int dy = opp_y - tank.getY();
        CanonDirection targetDirection = getDirectionFromDelta(dx, dy);
        int current = static_cast<int>(canon_direction);
        int target = static_cast<int>(targetDirection);
        int right_steps = (target - current + 8) % 8;
        int left_steps = (current - target + 8) % 8;
        if (left_steps <= right_steps) {
            for (int j = 0; j < left_steps; ++j) {
                moves.push_back(ActionRequest::ROTATE_EIGHTH_LEFT);
                canon_direction = rotateDirectionLeft(canon_direction);
            }
        } else {
            for (int j = 0; j < right_steps; ++j) {
                moves.push_back(ActionRequest::ROTATE_EIGHTH_RIGHT);
                canon_direction = rotateDirectionRight(canon_direction);
            }
        }
        moves.push_back(ActionRequest::SHOOT);
        continue;
        }
        //otherwise, follow the path
        for (size_t i = 1; i < current_path.size(); ++i) {
            int x1 = current_path[i - 1] / height;
            int y1 = current_path[i - 1] % height;
            int x2 = current_path[i] / height;
            int y2 = current_path[i] % height;
            int dx = x2 - x1;
            int dy = y2 - y1;

            CanonDirection targetDirection = getDirectionFromDelta(dx, dy);
            int current = static_cast<int>(canon_direction);
            int target = static_cast<int>(targetDirection);

            int right_steps = (target - current + 8) % 8;
            int left_steps = (current - target + 8) % 8;

            if (left_steps <= right_steps) {
                for (int j = 0; j < left_steps; ++j) {
                    moves.push_back(ActionRequest::ROTATE_EIGHTH_LEFT);
                    canon_direction = rotateDirectionLeft(canon_direction);
                }
            } else {
                for (int j = 0; j < right_steps; ++j) {
                    moves.push_back(ActionRequest::ROTATE_EIGHTH_RIGHT);
                    canon_direction = rotateDirectionRight(canon_direction);
                }
            }
            moves.push_back(ActionRequest::MOVE_FORWARD);
        }
    return moves;
}

ActionRequest Player1Algorithm::getAction() {
    if (!my_future_moves.empty()) {
        next_move = my_future_moves.front();
        my_future_moves.erase(my_future_moves.begin());
        return next_move;
        } else {
            return ActionRequest::GetBattleInfo;
        }
}

void Player1Algorithm::updateBattleInfo(BattleInfo& info) {
    auto& myInfo = dynamic_cast<Player1BattleInfo&>(info);  
    opponent_tanks = myInfo.opponents;
    shared_board = myInfo.board;
    
    // Now build the moves like you already do:
    std::vector<std::vector<int>> graph = getGraphOutOfBoard(shared_board);
    std::vector<int> path = getFutureMovesBFS(tank, graph, opponent_tanks);
    my_future_moves = getFutureMoves(path, tank, opponent_tanks, shared_board->getHeight());
}

std::vector<std::vector<int>>Player1Algorithm::getGraphOutOfBoard(const std::shared_ptr<GameBoard> board) {
    int width = board->getWidth();
    int height = board->getHeight();
    std::vector<std::vector<int>> graph(width * height + 1);
  
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!board->isCellPassable(x, y)) continue;
            int cell_index = x * height + y;

            // LEFT
            if (x > 0) {
                if (board->isCellPassable(x-1, y)) {
                    graph[cell_index].push_back((x-1) * height + y);
                } 
            }
            // RIGHT
            if (x < width - 1) {
                if (board->isCellPassable(x+1, y)) {
                    graph[cell_index].push_back((x+1) * height + y);
                } 
            }
            // UP
            if (y > 0) {
                if (board->isCellPassable(x, y-1)) {
                    graph[cell_index].push_back(x * height + (y-1));
                } 
            }
            // DOWN
            if (y < height - 1) {
                if (board->isCellPassable(x, y+1)) {
                    graph[cell_index].push_back(x * height + (y+1));
                } 
            }
        }
    }
    return graph;
}

int Player1Algorithm::isChased(const Player1& self, const std::shared_ptr<GameBoard> board) {
    //returns the index of the tank that is being chased or -1 if none
    std::vector<Tank> tanks = self.getTanks();
    std::vector<std::pair<int, int>> tanks_positions = {
        {tank.getX(), tank.getY()} for (const Tank& tank : tanks)
    };
    std::vector<auto> tanks_directions = {
        directionToVector(tank.getCanonDirection()) for (const Tank& tank : tanks)
    };
    std::vector<std::pair<int, int>> next_positions = {
        {tank.getX() + dx, tank.getY() + dy} for (const auto& [dx, dy] : tanks_directions)
    };

    for (const Shell& shell : board->getShells()) {
        int sx = shell.getX();
        int sy = shell.getY();
        auto sdir = directionToVector(shell.getDirection());
        int sdx = sdir.first;
        int sdy = sdir.second;

        for (int step = 0; step < 5; ++step) {
            int i =0;
            for(auto next_position : next_positions){
                int fx = next_position.first;
                int fy = next_position.second;
                if (sx == fx && sy == fy) return i;
            }
            sx += sdx;
            sy += sdy;
            i++;
            if (sx < 0 || sx >= board->getWidth() || sy < 0 || sy >= board->getHeight()) break;
        }
    }
    return -1;
}

bool Player1Algorithm::isFacingOpponent(const Tank& self, const Tank& opponent) {
    int dx = opponent.getX() - self.getX();
    int dy = opponent.getY() - self.getY();
    if (dx == 0 && dy == 0) return false;
    CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
    return self.getCanonDirection() == dirToOpponent;
}

bool Player1Algorithm::isFacingAnyOpponent(Player1 &player1, std::vector<Tank> opponent_tanks) {
    for (const Tank& tank : tanks) {
        for(const Tank& self : player1.getTanks()) {
            if (isFacingOpponent(self, tank)) {
                 return true;
            }
        }
    }
    return false;
}

int Player1Algorithm::canKillOpponent(int current_tank, std::vector<Tank> opponent_tanks) {
    //returns the index of the tank that is being chased or -1 if none
    std::vector<std::pair<int, int>> opponent_positions;
    for(const Tank& tank : opponent_tanks){
        opponent_positions.emplace_back(tank.getX(), tank.getY());
    }
    int i = 0;
    flat_position = current_tank;
        int x = flat_position / board->getHeight();
        int y = flat_position % board->getHeight();
        int i = 0;
        for(const auto& [oppx, oppy] : opponent_positions){
            int dx = oppx - x;
            int dy = oppy - y;
            if(dx == 0 || dy == 0 || std::abs(dx) == std::abs(dy)){
               return i;
            }
            i++;
        }
    return  -1;
}

bool Player1Algorithm::isTankFacingAnyOpponent(const Tank& self, std::vector<Tank> opponent_tanks) {
    // Check if the tank is facing any opponent tank
    for (const Tank& tank : opponent_tanks) {
        if (isFacingOpponent(self, tank)) {
            return true;
        }
    }
}