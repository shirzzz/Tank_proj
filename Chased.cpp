#include "Chased.h"
#include "DirectionUtils.h"
#include "common/ActionRequest.h"
#include "GameBoard.h"
#include "Tank.h"
#include "Shell.h"
#include <vector>

// // Check if any shell will move into the tile in front of the tank
// bool Chased::isDangerAhead() {
//     Tank& my_tank = *this->my_tank.get();
//     const GameBoard& board = *this->game_board.get();
//     std::pair<int, int> dir = directionToVector(tank.getCanonDirection());
//     int dx = dir.first;
//     int dy = dir.second;
//     int fx = tank.getX() + dx;
//     int fy = tank.getY() + dy;

//     for (const Shell& shell : board.getShells()) {
//         int sx = shell.getX();
//         int sy = shell.getY();
//         auto dir = directionToVector(shell.getDirection());
//         int sdx = dir.first;
//         int sdy = dir.second;

//         for (int step = 0; step < 5; ++step) {
//             if (sx == fx && sy == fy) return true;

//             sx += sdx;
//             sy += sdy;
//             if (sx < 0 || sx >= board.getWidth() || sy < 0 || sy >= board.getHeight())
//                 break;
//         }
//     }

//     return false;
// }

// // Helper function: check if opponent is aligned with self
// int isAlignedHorizontally() {
//     int i = 0;
//     //returns which opponent is aligned with self
//     for(const auto& opponent : opponents) {
//         if (my_tank.get().getY() == opponent.get().getY()) {
//             return i;
//         }
//         i++;
//     }
//     return -1; // No opponent aligned horizontally
// }

// int isAlignedVertically() {
//     int i = 0;
//     for(const auto& opponent : opponents) {
//         if (my_tank.get().getX() == opponent.get().getX()) {
//             return i;
//         }
//         i++;
//     }
//     return -1; // No opponent aligned vertically
// }

// // Helper: check if cannon direction matches direction to opponent
// int isFacingOpponent() {
//     Tank& self = *my_tank.get();
//     int i = 0;
//     for(const auto& opponent : opponents) {
//         if (opponent == nullptr) continue; // Skip null opponents
//         int dx = opponent.getX() - self.getX();
//         int dy = opponent.getY() - self.getY();
//         if (dx == 0 && dy == 0) return false; // Same position (somehow?)
//         CanonDirection dirToOpponent = getDirectionFromDelta(dx, dy);
//         if (self.getCanonDirection() == dirToOpponent) {
//             return i; // Facing opponent
//         }
//         i++;
//     }
//     return -1; // Not facing any opponent
// }

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
    //Tank& self = *my_tank.get();
    //GameBoard& board = *game_board.get();
    // // 1. Avoid danger if necessary
    // if (isDangerAhead()) {
    //     return rotateToward(self, opponents[0]);  // Or choose another evasive action
    // }
    // 2. If facing opponent and aligned, shoot!
    if ((isAlignedHorizontally() != -1 || isAlignedVertically() != -1) && isFacingOpponent() != -1) {
        return ActionRequest::Shoot;
    }

    // 3. Not facing opponent? Rotate toward them
    if (int index = isFacingOpponent() != -1) {
        return rotateToward(opponents[index]);
    }
    // 4. Default action: move toward the opponent
    //Itai here I need to use the canon direction of my tank
    //std::pair<int, int> next_cell = {(directionToVector(my_tank.get()->getCanonDirection())).first + my_tank.get()->getX(),
                                //(directionToVector(my_tank.get()->getCanonDirection())).second + my_tank.get()->getY()};
    //std::cout<<"board is nullptr? " << (game_board.get()->getCell(next_cell.first, next_cell.second) == nullptr) << std::endl;
    //std::cout<<game_board.get()->getCell(next_cell.first,next_cell.second)->getCellType() << std::endl;

    //Shir: Itai I decided that if a tank wants to die, it can die.
    // if(game_board.get()->getCell(next_cell.first,next_cell.second)->getCellType() == CellType::WALL) {
    //     return ActionRequest::RotateLeft45; // Or some other action
    // }
    // std::cout << "Checking next cell type." << std::endl;
    // if (game_board.get()->getCell(next_cell.first, next_cell.second)->getCellType() == CellType::MINE) {
    //     return ActionRequest::RotateLeft45; // Or some other action
    // }
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
    //my_future_moves.clear();
    for (int i = 0; i < 5; ++i) {
        my_future_moves.push_back(decideNextAction());
    }
}







