#include "Chased.h"
#include "DirectionUtils.h"
#include "common/ActionRequest.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>


// Compute best rotation toward the opponent's position
ActionRequest Chased::rotateToward(std::pair<size_t, size_t> opponent) {
    //Tank& self = *my_tank.get();
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
    // 1. If facing opponent and aligned, shoot!
    if (isFacingOpponent(currect_position_of_trail, current_direction) != -1) {
        return ActionRequest::Shoot;
    }

    // 3. Default action: move toward the opponent
    std::pair<int, int> next_cell = game_board.get()->getNextPosition(currect_position_of_trail.first, currect_position_of_trail.second, current_direction);

    if(!game_board.get()->isCellWalkable(next_cell.first, next_cell.second) || game_board.get()->isSteppingMine(next_cell.first, next_cell.second)) {
        switch(current_direction) {
            case CanonDirection::U: current_direction = CanonDirection::UR; break;
            case CanonDirection::UR: current_direction = CanonDirection::R; break;
            case CanonDirection::R: current_direction = CanonDirection::DR; break;
            case CanonDirection::DR: current_direction = CanonDirection::D; break;
            case CanonDirection::D: current_direction = CanonDirection::DL; break;
            case CanonDirection::DL: current_direction = CanonDirection::L; break;
            case CanonDirection::L: current_direction = CanonDirection::UL; break;
            case CanonDirection::UL: current_direction = CanonDirection::U; break;
        }
        return ActionRequest::RotateRight45; // If next cell is not walkable, rotate right
    }
    printf("Moving forward to (%d, %d)\n", next_cell.first, next_cell.second);
    std::cout<<game_board.get()->getCell(next_cell.first, next_cell.second)->getCellType()<<"\n";
    currect_position_of_trail.first = next_cell.first;
    currect_position_of_trail.second = next_cell.second;
    return ActionRequest::MoveForward;
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







