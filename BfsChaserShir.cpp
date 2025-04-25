
#include "BfsChaserShir.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "GameBoard.h"
#include "DirectionUtils.h"
#include "iostream"
std::vector<int> BfsChaserShir::getFutureMovesBfs(std::vector<std::vector<int>> graph, int start, int end) {
    std::vector<int> moves;
    std::queue<int> q;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;

    // Check if start and end are valid nodes
    if (start < 0 || static_cast<size_t>(start) >= graph.size() || 
        end < 0 || static_cast<size_t>(end) >= graph.size()) {
        // Return empty moves if invalid
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

        // Make sure current is within bounds of the graph
        if (current >= 0 && static_cast<size_t>(current) < graph.size()) {
            for (int neighbor : graph[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
    }

    // Only backtrack if we found a path
    if (pathFound) {
        int at = end;
        while (at != start) {
            // Check if the path is valid
            if (parent.find(at) == parent.end()) {
                // Path is broken, return empty vector
                return std::vector<int>();
            }
            moves.push_back(at);
            at = parent[at];
        }
        std::reverse(moves.begin(), moves.end());
    }
    
    return moves;
}
std::vector<ActionType> BfsChaserShir::getFutureMoves(std::vector<int> path, std::shared_ptr<Tank> tank1, std::shared_ptr<Tank> tank2) {
    std::vector<ActionType> moves;
    CanonDirection canon_direction = tank1->getCanonDirection();
    for (size_t i = 1; i < 6; ++i) {
        //the name of each cell is x*10 + y
        if(path == std::vector<int>()) {
            break;
        }
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

    // Since index = 10 * x + y, max possible index is 10*(width-1) + (height-1)
    // So vector needs to be large enough to accommodate this
    std::vector<std::vector<int>> graph(10 * width);  // this is safe since y < 10

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!board->isCellPassable(x, y)) continue;

            int cell_index = 10 * x + y;
            // LEFT
            if (x > 1 && board->isCellPassable(x-1, y)) {
                int neighbor = 10 * (x - 1) + y;
                graph[cell_index].push_back(neighbor);
            }

            // RIGHT
            if (x < 1*width - 1 && board->isCellPassable(x + 1, y)) {
               
                int neighbor = 10 * (x + 1) + y;
                graph[cell_index].push_back(neighbor);
            }

            // UP
            if (y > 0 && board->isCellPassable(x, y - 1)) {
                
                int neighbor = 10 * x + (y - 1);
                graph[cell_index].push_back(neighbor);
            }

            // DOWN
            if (y < height - 1 && board->isCellPassable(x, y + 1)) {
                
                int neighbor = 10 * x + (y + 1);
                graph[cell_index].push_back(neighbor);
            } 
        }
    }
;
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