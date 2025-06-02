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

std::vector<int> BfsChaserShir::getFutureMovesBfs(std::vector<std::vector<int>> graph, int start) {
    std::vector<int> moves;
    std::queue<int> q;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;

    if (start < 0 || static_cast<size_t>(start) >= graph.size() || end < 0 || static_cast<size_t>(end) >= graph.size()) {
        return moves;
    }

    q.push(start);
    visited.insert(start);

    bool pathFound = false;
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (canKillOpponent(current) != -1) {
            pathFound = true;
            break;
        }

        for (int neighbor : graph[current]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    if (pathFound) {
        int at = end;
        while (at != start) {
            if (parent.find(at) == parent.end()) {
                return std::vector<int>();
            }
            moves.push_back(at);
            at = parent[at];
        }
        std::reverse(moves.begin(), moves.end());
    }

    return moves;
}

void BfsChaserShir::setFutureMoves(std::vector<int> path, int height) {
    if (path.empty()) {
        my_future_moves.push_back(ActionRequest::Shoot);
        return;
    }

    CanonDirection canon_direction = my_tank.get()->getCanonDirection();

    for (size_t i = 1; i < path.size(); ++i) {
        int x1 = path[i - 1] / height;
        int y1 = path[i - 1] % height;
        int x2 = path[i] / height;
        int y2 = path[i] % height;

        // 1. Check if already facing an opponent
        if (isFacingOpponent() != -1) {
            my_future_moves.push_back(ActionRequest::Shoot);
            continue;
        }

        // 2. Check if can kill any opponent from this tile
        int index = canKillOpponent(path[i]);
        if (index != -1) {
            std::pair<int, int> pos = fromIndextoPos(path[i], height);
            int dx = opponents[index].first- pos.first;
            int dy = opponents[index].second - pos.second;
            CanonDirection target_direction = getDirectionFromDelta(dx, dy);
            
            if (canon_direction == target_direction) {
                my_future_moves.push_back(ActionRequest::Shoot);
                continue;
            } else {
                int current = static_cast<int>(canon_direction);
                int target = static_cast<int>(target_direction);
                int right_steps = (target - current + 8) % 8;
                int left_steps = (current - target + 8) % 8;

                if (left_steps <= right_steps) {
                    for (int j = 0; j < left_steps; ++j) {
                        my_future_moves.push_back(ActionRequest::RotateLeft45);
                        canon_direction = rotateDirectionLeft(canon_direction);
                    }
                } else {
                    for (int j = 0; j < right_steps; ++j) {
                        my_future_moves.push_back(ActionRequest::RotateRight45);
                        canon_direction = rotateDirectionRight(canon_direction);
                    }
                }

                my_future_moves.push_back(ActionRequest::Shoot);
                continue;
            }
        }

        // 3. Move forward in the path
        int dx = x2 - x1;
        int dy = y2 - y1;
        CanonDirection target_direction = getDirectionFromDelta(dx, dy);
        int current = static_cast<int>(canon_direction);
        int target = static_cast<int>(target_direction);

        int right_steps = (target - current + 8) % 8;
        int left_steps = (current - target + 8) % 8;

        if (left_steps <= right_steps) {
            for (int j = 0; j < left_steps; ++j) {
                my_future_moves.push_back(ActionRequest::RotateLeft45);
                canon_direction = rotateDirectionLeft(canon_direction);
            }
        } else {
            for (int j = 0; j < right_steps; ++j) {
                my_future_moves.push_back(ActionRequest::RotateRight45);
                canon_direction = rotateDirectionRight(canon_direction);
            }
        }

        my_future_moves.push_back(ActionRequest::MoveForward);
    }
}

ActionRequest BfsChaserShir::getAction() {
    if (!my_future_moves.empty()) {
        ActionRequest next_move = my_future_moves.front();
        my_future_moves.erase(my_future_moves.begin());
        return next_move;
    } else {
        MyBattleInfo battle_info(my_tank.get()->getSatelliteView(), my_tank.get()->getIndexTank());
        game_board = battle_info.getGameBoard();
        opponents = battle_info.getOpponents();
        std::vector<std::vector<int>> graph = getGraphOutOfBoard();

        int start_node = my_tank.get()->getX() * game_board->getHeight() + my_tank.get()->getY();

        std::vector<int> path = getFutureMovesBfs(graph, start_node);
       setFutureMoves(path, game_board->getHeight());
        //need to check with shir
        if (my_future_moves.empty()) {
            //Shir: I believe it's better to shoot if we have no future moves
            return ActionRequest::DoNothing;
        }

        ActionRequest next_move = my_future_moves.front();
        my_future_moves.erase(my_future_moves.begin());

        std::cout << "Next move: " << static_cast<int>(next_move) << std::endl;
        std::cout<<"at the end of the function"<<std::endl;
        return next_move;
    }
}

std::vector<std::vector<int>> BfsChaserShir::getGraphOutOfBoard(const std::shared_ptr<GameBoard> board) {
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

bool BfsChaserShir::isChased(const Tank& self, const std::shared_ptr<GameBoard> board) {
    int tx = self.getX();
    int ty = self.getY();
    auto dir = directionToVector(self.getCanonDirection());
    int dx = dir.first;
    int dy = dir.second;

    int fx = tx + dx;
    int fy = ty + dy;

    for (const Shell& shell : board->getShells()) {
        int sx = shell.getX();
        int sy = shell.getY();
        auto sdir = directionToVector(shell.getDirection());
        int sdx = sdir.first;
        int sdy = sdir.second;

        for (int step = 0; step < 5; ++step) {
            if (sx == fx && sy == fy) return true;
            sx += sdx;
            sy += sdy;
            if (sx < 0 || sx >= board->getWidth() || sy < 0 || sy >= board->getHeight()) break;
        }
    }
    return false;
}

int canKillOpponent(int spot) {
    std::pair<int, int> pos = fromIndextoPos(spot, shared_board->getHeight());
    int i = 0;
    for (std::pair<size_t, size_t> opponent : opponents) {
        if (opponent == nullptr) continue; // Skip null opponents
        int dx = opponent.first - pos.first;
        int dy = opponent.second - pos.second;
        if (dx == 0 || dy == 0 || abs(dx) == abs(dy)) return i; // Same position'
        i++;
    }
    return -1; // No opponent can be
}

vector<int> opponentsSpots() {
    vector<int> opponents_spots;
    for (std::pair<size_t, size_t> opponent : opponents) {
        if (opponent == nullptr) continue; // Skip null opponents
        int pos = opponent.first * shared_board->getHeight() + opponent.second;
        opponents_spots.push_back(pos);
    }
    return opponents_spots;
}

std::pair<int, int> fromIndextoPos const (int index, int height) {
    return {index / height, index % height};
}