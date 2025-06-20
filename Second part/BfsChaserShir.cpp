#include "BfsChaserShir.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include "GameBoard.h"
#include "DirectionUtils.h"

std::pair<int, int> BfsChaserShir::fromIndextoPos (int index, int height) {
    return {index / height, index % height};
}

int BfsChaserShir::canKillOpponent(int spot) {
    std::pair<int, int> pos = fromIndextoPos(spot, height);
    int i = 0;
    for (std::pair<size_t, size_t> opponent : opponents) {
        int dx = opponent.first - pos.first;
        int dy = opponent.second - pos.second;
        if (dx == 0 || dy == 0 || abs(dx) == abs(dy)){
            return i; // Same position'
        } 
        i++;
    }
    return -1; // No opponent can be
}


CanonDirection rotateDirectionLeft(CanonDirection dir) {
    return static_cast<CanonDirection>((static_cast<int>(dir) + 7) % 8);
}

CanonDirection rotateDirectionRight(CanonDirection dir) {
    return static_cast<CanonDirection>((static_cast<int>(dir) + 1) % 8);
}

std::vector<int> BfsChaserShir::getFutureMovesBfs(const std::vector<std::vector<int>>& graph, int start) {
    std::vector<int> moves;
    std::queue<int> q;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;
    int end = -1; // To store the end node where we can kill an opponent
    if (start < 0 || static_cast<size_t>(start) >= graph.size() ) {
        return moves;
    }

    q.push(start);
    visited.insert(start);

    bool pathFound = false;
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (canKillOpponent(current) != -1) {
            end = current; // Found a path to an opponent
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
    if (moves.empty()) {
        return moves; // Return empty vector if no path found
    }
    return moves;
}

void BfsChaserShir::setFutureMoves(const std::vector<int>& path, int height, int start) {
    if (path.empty()) {
        handleEmptyPath(start, height);
        return;
    }

    for (size_t i = 1; i < path.size(); ++i) {
        int x1 = path[i - 1] / height;
        int y1 = path[i - 1] % height;
        int x2 = path[i] / height;
        int y2 = path[i] % height;

        if (isFacingOpponent() != -1) {
            my_future_moves.push_back(ActionRequest::Shoot);
            continue;
        }

        if (tryToShootOpponent(path[i], height))
            continue;
        int dx = x2 - x1;
        int dy = y2 - y1;
        CanonDirection target_direction = getDirectionFromDelta(dx, dy);
        if(current_direction == target_direction){
            my_future_moves.push_back(ActionRequest::MoveForward);
            return;
        }
        rotateTowardsAndMoveForward(x1, y1, x2, y2);
    }
}

void BfsChaserShir::handleEmptyPath(int start, int height) {
    int index = canKillOpponent(start);
    if (index == -1) {
        my_future_moves.push_back(ActionRequest::DoNothing);
        return;
    }

    std::pair<int, int> pos = fromIndextoPos(start, height);
    int dx = opponents[index].first - pos.first;
    int dy = opponents[index].second - pos.second;
    CanonDirection target_direction = getDirectionFromDelta(dx, dy);

    rotateCanonTowards(target_direction);
    my_future_moves.push_back(ActionRequest::Shoot);
}

bool BfsChaserShir::tryToShootOpponent(int indexPos, int height) {
    int index = canKillOpponent(indexPos);
    if (index == -1) return false;

    std::pair<int, int> pos = fromIndextoPos(indexPos, height);
    int dx = opponents[index].first - pos.first;
    int dy = opponents[index].second - pos.second;
    CanonDirection target_direction = getDirectionFromDelta(dx, dy);

    rotateCanonTowards(target_direction);
    my_future_moves.push_back(ActionRequest::Shoot);
    return true;
}

void BfsChaserShir::rotateTowardsAndMoveForward(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    CanonDirection target_direction = getDirectionFromDelta(dx, dy);

    rotateCanonTowards(target_direction);
    my_future_moves.push_back(ActionRequest::MoveForward);
}

void BfsChaserShir::rotateCanonRight45() {
    switch (current_direction) {
        case CanonDirection::U:  current_direction = CanonDirection::UR; break;
        case CanonDirection::UR: current_direction = CanonDirection::R;  break;
        case CanonDirection::R:  current_direction = CanonDirection::DR; break;
        case CanonDirection::DR: current_direction = CanonDirection::D;  break;
        case CanonDirection::D:  current_direction = CanonDirection::DL; break;
        case CanonDirection::DL: current_direction = CanonDirection::L;  break;
        case CanonDirection::L:  current_direction = CanonDirection::UL; break;
        case CanonDirection::UL: current_direction = CanonDirection::U;  break;
    }
}

void BfsChaserShir::rotateCanonLeft45() {
    switch (current_direction) {
        case CanonDirection::U:  current_direction = CanonDirection::UL; break;
        case CanonDirection::UL: current_direction = CanonDirection::L;  break;
        case CanonDirection::L:  current_direction = CanonDirection::DL; break;
        case CanonDirection::DL: current_direction = CanonDirection::D;  break;
        case CanonDirection::D:  current_direction = CanonDirection::DR; break;
        case CanonDirection::DR: current_direction = CanonDirection::R;  break;
        case CanonDirection::R:  current_direction = CanonDirection::UR; break;
        case CanonDirection::UR: current_direction = CanonDirection::U;  break;
    }
}

void BfsChaserShir::rotateCanonRight90() {
    rotateCanonRight45();
    rotateCanonRight45();
}

void BfsChaserShir::rotateCanonLeft90() {
    rotateCanonLeft45();
    rotateCanonLeft45();
}


void BfsChaserShir::rotateCanonTowards(CanonDirection target_direction) {
    int current = static_cast<int>(current_direction);
    int target = static_cast<int>(target_direction);
    int right_steps = (target - current + 8) % 8;
    int left_steps = (current - target + 8) % 8;

    if (left_steps <= right_steps) {
        for (int j = 0; j < left_steps; ++j){
            rotateCanonLeft45();
            my_future_moves.push_back(ActionRequest::RotateLeft45);
        }
    } else {
        for (int j = 0; j < right_steps; ++j){
            rotateCanonRight45();
            my_future_moves.push_back(ActionRequest::RotateRight45);
        }
    }
}


ActionRequest BfsChaserShir::getAction() {
    if (!my_future_moves.empty()) {
        ActionRequest next_move = my_future_moves.front();
        my_future_moves.erase(my_future_moves.begin());
        return next_move;
    } else {
        if (have_battle_info) {
            std::vector<std::vector<int>> graph = getGraphOutOfBoard();
            int start_node = my_tank.get()->getX() * height+ my_tank.get()->getY();
            std::vector<int> path = getFutureMovesBfs(graph, start_node);
            current_direction = my_tank.get()->getCanonDirection();
            setFutureMoves(path,height, start_node);

            if (my_future_moves.empty()) {
                return ActionRequest::DoNothing;
            }
            ActionRequest next_move = my_future_moves.front();
            my_future_moves.erase(my_future_moves.begin());
            return next_move;
        } else {
            return ActionRequest::GetBattleInfo; // If no battle info, request it
        }
    }
}

std::vector<std::vector<int>> BfsChaserShir::getGraphOutOfBoard() {
    std::vector<std::vector<int>> graph(width * height + 1);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!game_board->isCellPassable(x, y)) continue;
            int cell_index = x * height + y;

            // LEFT
            if (x > 0) {
                if (game_board->isCellPassable(x-1, y)) {
                    graph[cell_index].push_back((x-1) * height + y);
                } 
            }
            // RIGHT
            if (x < width - 1) {
                if (game_board->isCellPassable(x+1, y)) {
                    graph[cell_index].push_back((x+1) * height + y);
                } 
            }
            // UP
            if (y > 0) {
                if (game_board->isCellPassable(x, y-1)) {
                    graph[cell_index].push_back(x * height + (y-1));
                } 
            }
            // DOWN
            if (y < height - 1) {
                if (game_board->isCellPassable(x, y+1)) {
                    graph[cell_index].push_back(x * height + (y+1));
                } 
            }
        }
    }
    return graph;
}

std::vector<int> BfsChaserShir::opponentsSpots() {
    std::vector<int> opponents_spots;
    for (std::pair<size_t, size_t> opponent : opponents) {
        int pos = opponent.first *height + opponent.second;
        opponents_spots.push_back(pos);
    }
    return opponents_spots;
}

