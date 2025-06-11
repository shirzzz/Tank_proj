#include "Chased.h"
#include "DirectionUtils.h"
#include "common/ActionRequest.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>

int Chased::canKillOpponent(std::pair<int, int> pos) {
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

// Compute best rotation toward the opponent's position
ActionRequest Chased::rotateToward(std::pair<size_t, size_t> opponent) {
    int dx = opponent.first - my_tank.get()->getX();
    int dy = opponent.second - my_tank.get()->getY();
    CanonDirection desiredDir = getDirectionFromDelta(dx, dy);
    CanonDirection currentDir = my_tank.get()->getCanonDirection(); // Use the tank's canon direction Itai 

    if (currentDir == desiredDir) return ActionRequest::Shoot;

    int curIdx = static_cast<int>(currentDir);
    int desIdx = static_cast<int>(desiredDir);
    int diff = (desIdx - curIdx + 8) % 8;

    if (diff == 1 || diff == 2) return ActionRequest::RotateRight45;
    if (diff == 6 || diff == 7) return ActionRequest::RotateLeft45;
    if (diff == 3 || diff == 4) return ActionRequest::RotateRight90;
    return ActionRequest::RotateLeft90;
}

ActionRequest Chased::decideNextAction() {
    if (isFacingOpponent(currect_position_of_trail, current_direction) != -1) {
        return ActionRequest::Shoot;
    }

    int index = canKillOpponent(currect_position_of_trail);
    if (index != -1) {
        return rotateTowardAndUpdate(index);
    }

    return moveTowardOpponent();
}

ActionRequest Chased::rotateTowardAndUpdate(int index) {
    ActionRequest action = rotateToward(opponents[index]);
    switch (action) {
        case ActionRequest::RotateRight45:
            rotateCanonRight45(); break;
        case ActionRequest::RotateRight90:
            rotateCanonRight90(); break;
        case ActionRequest::RotateLeft45:
            rotateCanonLeft45(); break;
        case ActionRequest::RotateLeft90:
            rotateCanonLeft90(); break;
        default:
            break;
    }
    return action;
}

ActionRequest Chased::moveTowardOpponent() {
    auto next_cell = game_board.get()->getNextPosition(
        currect_position_of_trail.first,
        currect_position_of_trail.second,
        current_direction
    );

    if (!game_board.get()->isCellWalkable(next_cell.first, next_cell.second) ||
        game_board.get()->isSteppingMine(next_cell.first, next_cell.second)) {
        rotateCanonRight45();
        return ActionRequest::RotateRight45;
    }

    printf("Moving forward to (%d, %d)\n", next_cell.first, next_cell.second);
    std::cout << game_board.get()->getCell(next_cell.first, next_cell.second)->getCellType() << "\n";
    currect_position_of_trail = next_cell;
    return ActionRequest::MoveForward;
}

void Chased::rotateCanonRight45() {
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

void Chased::rotateCanonLeft45() {
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

void Chased::rotateCanonRight90() {
    rotateCanonRight45();
    rotateCanonRight45();
}

void Chased::rotateCanonLeft90() {
    rotateCanonLeft45();
    rotateCanonLeft45();
}

ActionRequest Chased::getAction() {
    if (my_future_moves.empty()) {
        if(have_battle_info) {
            setFutureMoves();
            if (my_future_moves.empty()) {
                return ActionRequest::DoNothing; // If no future moves, do nothing
            } else {
            ActionRequest action = my_future_moves.front();
            my_future_moves.erase(my_future_moves.begin());
            return action;
            }
        }else{
            return ActionRequest::GetBattleInfo; // If no battle info, request it
        }  
    }
    ActionRequest next_move = my_future_moves.front();
    my_future_moves.erase(my_future_moves.begin());
    return next_move;
}
void Chased::setFutureMoves() {
    my_future_moves.clear(); // Clear previous moves
    currect_position_of_trail = my_tank.get()->getPosition();
    current_direction = my_tank.get()->getCanonDirection(); // Get the current direction of the tank
    for (int i = 0; i < 5; ++i) {
        my_future_moves.push_back(decideNextAction());
    }
}







