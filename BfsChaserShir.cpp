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

std::vector<int> BfsChaserShir::getFutureMovesBfs(std::vector<std::vector<int>> graph, int start, int end) {
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

        if (current == end) {
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

std::vector<ActionType> BfsChaserShir::getFutureMoves(std::vector<int> path, std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2, int height) {
    std::vector<ActionType> moves;

    if (path.empty()) {
        moves.push_back(ActionType::SHOOT);
        return moves;
    }

    CanonDirection canon_direction = tank1->getCanonDirection();

    for (size_t i = 1; i < path.size(); ++i) {
        int x1 = path[i-1] / height;
        int y1 = path[i-1] % height;
        int x2 = path[i] / height;
        int y2 = path[i] % height;
        int dx = x2 - x1;
        int dy = y2 - y1;

        if (isFacingOpponent(*tank1, *tank2)) {
            moves.push_back(ActionType::SHOOT);
            continue;
        }

        CanonDirection targetDirection = getDirectionFromDelta(dx, dy);
        int current = static_cast<int>(canon_direction);
        int target = static_cast<int>(targetDirection);

        int right_steps = (target - current + 8) % 8;
        int left_steps = (current - target + 8) % 8;

        if (left_steps <= right_steps) {
            for (int j = 0; j < left_steps; ++j) {
                moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                canon_direction = rotateDirectionLeft(canon_direction);
            }
        } else {
            for (int j = 0; j < right_steps; ++j) {
                moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                canon_direction = rotateDirectionRight(canon_direction);
            }
        }

        moves.push_back(ActionType::MOVE_FORWARD);
    }

    return moves;
}

ActionType BfsChaserShir::getNextMove(std::shared_ptr<GameBoard> shared_board, std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2) {
    if (!my_future_moves.empty()) {
        ActionType next_move = my_future_moves.front();
        my_future_moves.erase(my_future_moves.begin());
        return next_move;
    } else {
        std::vector<std::vector<int>> graph = getGraphOutOfBoard(shared_board);

        int startNode = tank1->getX() * shared_board->getHeight() + tank1->getY();
        int endNode = tank2->getX() * shared_board->getHeight() + tank2->getY();

        std::vector<int> path = getFutureMovesBfs(graph, startNode, endNode);
        my_future_moves = getFutureMoves(path, tank1, tank2, shared_board->getHeight());

        if (my_future_moves.empty()) {
            return ActionType::INVALID_ACTION;
        }

        ActionType next_move = my_future_moves.front();
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

bool BfsChaserShir::isFacingOpponent(const Tank& self, const Tank& opponent) {
    int dx = opponent.getX() - self.getX();
    int dy = opponent.getY() - self.getY();
    if (dx == 0 && dy == 0) return false;
    CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
    return self.getCanonDirection() == dirToOpponent;
}
