#include "BfsChaserShir.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include "GameBoard.h"
#include "DirectionUtils.h"
namespace Algorithm_211466123_212399455{
//to add chasing from shells

bool BfsChaserShir::amIshootingMyTeammates() const {
    if (!game_board) return false; // No game board available
    std::pair<int, int> delta = directionToVector(my_tank->getCanonDirection());
    int dx = delta.first;
    int dy = delta.second;
    int x = my_tank.get()->getX();
    int y = my_tank.get()->getY();
    while (true) {
        x  = (x + dx)  % game_board->getWidth();
        y = (y + dy)  % game_board->getHeight();
        auto cell = game_board->getCell(x, y);
        if (!cell || cell->getCellType() == CellType::WALL || cell->getCellType() == CellType::TANK1 || cell->getCellType() == CellType::TANK2 || cell->getCellType() == CellType::SHELL) {
            if(my_tank->getIndexTank() == '1' && cell->getCellType() == CellType::TANK1) {
                return true; // Shooting at a teammate
            } else if(my_tank->getIndexTank() == '2' && cell->getCellType() == CellType::TANK2) {
                return true; // Shooting at a teammate
            }
            if(my_tank->getIndexTank() == '1' && cell->getCellType() == CellType::TANK2) {
                return false; // Shooting at an opponent
            } else if(my_tank->getIndexTank() == '2' && cell->getCellType() == CellType::TANK1) {
                return false; // Shooting at an opponent
            }
            if(cell->getCellType() == CellType::SHELL) {
                return false; // Shooting at a shell, not a teammate
            }
            if(cell->getCellType() == CellType::WALL) {
                return false; // Shooting at a wall, not a teammate
            }
        }
        else{
            continue; // Continue until we hit a wall or a tank
        }
    }
    return false; // Not facing the tank
}

bool BfsChaserShir::isShellThreateningMe(const Shell& shell) const {
    // Check if the shell is threatening my tank
    if (!game_board) return false; // No game board available
    std::pair<int, int> delta = directionToVector(shell.getDirection());
    int dx = delta.first;
    int dy = delta.second;
    int x = shell.getX();
    int y = shell.getY();
    while (true) {
        x  = (x + dx)  % game_board->getWidth();
        y = (y + dy)  % game_board->getHeight();
        auto cell = game_board->getCell(x, y);
        switch(cell->getCellType()) {
            case CellType::WALL:
                return false; // Shell hit a wall, not threatening
            case CellType::TANK1:
            case CellType::TANK2:
                if (cell->getX() == my_tank->getX() && cell->getY() == my_tank->getY()) {
                    return true; // Shell is threatening my tank
                }
                return false; // Shell hit a tank, not threatening
            case CellType::SHELL:
                return false; // Shell hit another shell, not threatening
            default:
                continue; // Continue until we hit a wall or a tank
        }
    }
}

std::vector<Shell> BfsChaserShir::shouldRunAwayFromShells() const {
    //Function which returns a vector of shells that are threatening the tank
    std::vector<Shell> shells_to_run_from;
    if (!game_board) return shells_to_run_from; // No game board available
    for (const Shell& shell : game_board->getShells()) {
        if (isShellThreateningMe(shell)) {
            shells_to_run_from.push_back(shell);
        }
    }
    return shells_to_run_from;
}

bool BfsChaserShir::shouldIShootShell(const Shell& shell) const {
    // Function to check if the tank should shoot at the shell
    if (!game_board) return false; // No game board available
    std::pair<int, int> delta = directionToVector(my_tank->getCanonDirection());
    int dx = delta.first;
    int dy = delta.second;
    int x = my_tank->getX();
    int y = my_tank->getY();
    while (true) {
        x  = (x + dx)  % game_board->getWidth();
        y = (y + dy)  % game_board->getHeight();
        auto cell = game_board->getCell(x, y);
        if(!cell && cell->getX() == shell.getX() && cell->getY() == shell.getY()) {
                return true; // Shell is in the line of fire
            }
        if (!cell && cell->getCellType() == CellType::WALL || cell->getCellType() == CellType::TANK1 || cell->getCellType() == CellType::TANK2) {
            return false; // Not in the line of fire
        }
    }
}

void BfsChaserShir::setSafeEscapeMoves(std::vector<Shell>& shells){
    // Function to set safe escape moves from threatening shells
    if (!game_board) return; // No game board available
    for (Shell& shell : shells) {
        if (shouldIShootShell(shell)) {
            my_future_moves.push_back(ActionRequest::Shoot);
        } else {
            std::pair<int, int> shell_pos = shell.getPosition();
            int dx = shell_pos.first - my_tank->getX();
            int dy = shell_pos.second - my_tank->getY();
            CanonDirection escape_direction = findEscapeDirection(shells, 3); // Assuming a danger radius of 3
            if(escape_direction == my_tank->getCanonDirection()) {
                my_future_moves.push_back(ActionRequest::MoveForward);
            } else {
                // Rotate canon towards the escape direction
                rotateCanonTowards(escape_direction);
                my_future_moves.push_back(ActionRequest::MoveForward);
            }
        }
    }
}

CanonDirection BfsChaserShir::findEscapeDirection(const std::vector<Shell>& shells, int danger_radius) const {
    // Function to find a safe escape direction from threatening shells
    if (!game_board) return CanonDirection::U; // No game board available
    std::pair<int, int> current_pos = my_tank->getPosition();
    std::vector<CanonDirection> escape_directions = {
        CanonDirection::U, CanonDirection::UR, CanonDirection::R,
        CanonDirection::DR, CanonDirection::D, CanonDirection::DL,
        CanonDirection::L, CanonDirection::UL
    };
    for(auto& direction : escape_directions) {
        std::pair<int, int> delta = directionToVector(direction);
        int new_x = (current_pos.first + delta.first + game_board->getWidth()) % game_board->getWidth();
        int new_y = (current_pos.second + delta.second + game_board->getHeight()) % game_board->getHeight();
        if (game_board->isCellWalkable(new_x, new_y)) {
            bool safe = true;
            for (const Shell& shell : shells) {
                int shell_x = shell.getX();
                int shell_y = shell.getY();
                std::pair<int, int> shell_direction = directionToVector(shell.getDirection());
                int shell_dx = shell_direction.first;
                int shell_dy = shell_direction.second;
                for(int step = 0; step <= danger_radius; ++step) {
                    int next_shell_x = (shell_x + step * shell_dx) % game_board->getWidth();
                    int next_shell_y = (shell_y + step * shell_dy) % game_board->getHeight();
                    if (next_shell_x == new_x && next_shell_y == new_y) {
                        safe = false; // Shell is threatening this position
                        break;
                    }
                }
            }
            if (safe) {
                return direction; // Found a safe escape direction
            }
        }
    }
    return my_tank->getCanonDirection(); // Default direction if no escape found
}




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
            if(!amIshootingMyTeammates()){
                my_future_moves.push_back(ActionRequest::Shoot);
            }
            else{
                my_future_moves.push_back(ActionRequest::GetBattleInfo);
            }
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
        std::vector<Shell> shells_threatening = shouldRunAwayFromShells();
        if (shells_threatening.size() > 0) {
            my_future_moves.clear(); // Clear future moves if we need to run away from shells
            setSafeEscapeMoves(shells_threatening);
        }
        ActionRequest next_move = my_future_moves.front();
        my_future_moves.erase(my_future_moves.begin());
        return next_move;
    } else {
        if (have_battle_info) {
            std::vector<Shell> shells_threatening = shouldRunAwayFromShells();
            if(shells_threatening.size() > 0) {
                setSafeEscapeMoves(shells_threatening);
                if (!my_future_moves.empty()) {
                    my_future_moves.clear();               
                }
                    setSafeEscapeMoves(shells_threatening);
                    ActionRequest next_move = my_future_moves.front();
                    my_future_moves.erase(my_future_moves.begin());
                    return next_move;
                }
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
}
