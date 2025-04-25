
#include "BfsChaserShir.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "GameBoard.h"
#include "DirectionUtils.h"
std::vector<int> BfsChaserShir::getFutureMovesBfs(std::vector<std::vector<int>> graph, int start, int end) {
    std::vector<int> moves;
    std::queue<int> q;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == end) {
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

    // Backtrack to find the path
    for (int at = end; at != start; at = parent[at]) {
        moves.push_back(at);
    }
    std::reverse(moves.begin(), moves.end());
    return moves;
}

std::vector<ActionType> BfsChaserShir::getFutureMoves(std::vector<int> path, std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2) {
    std::vector<ActionType> moves;
    CanonDirection canon_direction = tank1->getCanonDirection();
    for (size_t i = 1; i < 6; ++i) {
        //the name of each cell is x*10 + y
        int dx = path[i] / 10 - path[i - 1] / 10;
        int dy = path[i] % 10 - path[i - 1] % 10;
        if(isFacingOpponent(*tank1.get(), *tank2.get())) {
            moves.push_back(ActionType::SHOOT);
            continue;
        }
        switch (canon_direction) {
            case CanonDirection::U:
               if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if(dx == 1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if(dx == 1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if(dx == 1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if(dx == -1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if(dx == -1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if(dx == -1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                }
                break;
            case CanonDirection::UR:
                if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if(dx == 1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if(dx == 1 && dy == 1) {
                    moves.push_back(ActionType::MOVE_FORWARD);
                } else if(dx == 1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if(dx == -1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if(dx == -1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if(dx == -1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                }
                break;
            case CanonDirection::R:
                if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 1 && dy == 0) {
                    moves.push_back(ActionType::MOVE_FORWARD);
                } else if (dx == 1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if (dx == -1 && dy == 0) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if (dx == -1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == -1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                }
                break;

            case CanonDirection::DR:
                if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == 1 && dy == -1) {
                    moves.push_back(ActionType::MOVE_FORWARD);
                } else if (dx == -1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == -1 && dy == 1) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if (dx == -1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                }
                break;

            case CanonDirection::D:
                if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::MOVE_FORWARD);
                } else if (dx == 1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == 1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == -1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == -1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == -1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                }
                break;

            case CanonDirection::DL:
                if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if (dx == 1 && dy == 1) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if (dx == 1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == -1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if (dx == -1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == -1 && dy == -1) {
                    moves.push_back(ActionType::MOVE_FORWARD);
                }
                break;

            case CanonDirection::L:
                if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                } else if (dx == 1 && dy == 0) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if (dx == 1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if (dx == 1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == -1 && dy == 0) {
                    moves.push_back(ActionType::MOVE_FORWARD);
                } else if (dx == -1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == -1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                }
            break;

            case CanonDirection::UL:
                if (dx == 0 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 0 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if (dx == 0 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_RIGHT);
                } else if (dx == 1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == 1 && dy == 1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_RIGHT);
                } else if (dx == 1 && dy == -1) {
                    moves.push_back(ActionType::MOVE_BACKWARD);
                } else if (dx == -1 && dy == 0) {
                    moves.push_back(ActionType::ROTATE_EIGHTH_LEFT);
                } else if (dx == -1 && dy == 1) {
                    moves.push_back(ActionType::MOVE_FORWARD);
                } else if (dx == -1 && dy == -1) {
                    moves.push_back(ActionType::ROTATE_QUARTER_LEFT);
                }
            break;
        }
    }
    return moves;
}
ActionType BfsChaserShir::getNextMove(std::shared_ptr<GameBoard> shared_board, std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2) {
    if(!my_future_moves.empty()) {
        ActionType next_move = my_future_moves[0];
        my_future_moves.erase(my_future_moves.begin());
        return next_move;
    }
    else {
        std::vector<std::vector<int>> graph = getGraphOutOfBoard(shared_board);
        std::vector<int> path = getFutureMovesBfs(graph, tank1->getX() * 10 + tank1->getY(), tank2->getX() * 10 + tank2->getY());
        my_future_moves = getFutureMoves(path, tank1, tank2);
    }
    return my_future_moves[0];
}

std::vector<std::vector<int>> BfsChaserShir::getGraphOutOfBoard(const std::shared_ptr<GameBoard> board) {
    int width = board->getWidth();
    int height = board->getHeight();
    std::vector<std::vector<int>> graph(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (board->isCellWalkable(x, y)) {
                int cell_index = x * 10 + x;
                if (x > 0 && board->isCellWalkable(x - 1, y)) {
                    graph[cell_index].push_back(cell_index - 10);
                    graph[(x-1) * 10 + y].push_back(cell_index);
                }
                if (x < width - 1 && board->isCellWalkable(x + 1, y)) {
                    graph[cell_index].push_back(cell_index + 10);
                    graph[(x+1) * 10 + y].push_back(cell_index);
                }
                if (y > 0 && board->isCellWalkable(x, y - 1)) {
                    graph[cell_index].push_back(x*10 + (y-1));
                    graph[x * 10 + (y-1)].push_back(cell_index);
                }
                if (y < height - 1 && board->isCellWalkable(x, y + 1)) {
                    graph[cell_index].push_back(x*10 + (y+1));
                    graph[x * 10 + (y+1)].push_back(cell_index);
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

    // Check each shell to see if it will pass through (fx, fy)
    for (const Shell& shell : board.get()->getShells()) {
        int sx = shell.getX();
        int sy = shell.getY();
        auto sdir = directionToVector(shell.getDirection());
        int sdx = sdir.first;
        int sdy = sdir.second;
        

        // Simulate 5 future steps of shell movement
        for (int step = 0; step < 5; ++step) {
            if (sx == fx && sy == fy) return true;
            sx += sdx;
            sy += sdy;

            if (sx < 0 || sx >= board.get()->getWidth() || sy < 0 || sy >= board.get()->getHeight()) break;
        }
    }

    return false;
}

bool BfsChaserShir::isFacingOpponent(const Tank& self, const Tank& opponent) {
    int dx = opponent.getX() - self.getX();
    int dy = opponent.getY() - self.getY();

    if (dx == 0 && dy == 0) return false; // Same position (somehow?)

    CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
    return self.getCanonDirection() == dirToOpponent;
}