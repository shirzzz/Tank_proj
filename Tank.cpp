#include "Tank.h"
#include "CanonDirection.h"
#include "Chased.h"
#include "Shape.h"
#include "DestructionCause.h"

#include <iostream>

Tank::Tank(int x, int y, char index_tank)
    : Shape(x, y), index_tank(index_tank),num_bullets(16){
    if (index_tank == '1') {
        canon_dir = CanonDirection::L;
        this->cell_type = CellType::TANK1;
    } else {
        canon_dir = CanonDirection::R;
        this->cell_type = CellType::TANK2;
    }
}


void Tank::shoot() {
    if (num_bullets > 0) {
        num_bullets--;
        // Shoot logic can be extended here
    } else {
        std::cout << "No bullets left!" << std::endl;
    }
}

// Movement
std::pair<int, int> Tank::moveForward(int board_width, int board_height) {
    int x_moved = x, y_moved = y;

    switch (canon_dir) {
        case CanonDirection::U:  y_moved--; break;
        case CanonDirection::D:  y_moved++; break;
        case CanonDirection::L:  x_moved--; break;
        case CanonDirection::R:  x_moved++; break;
        case CanonDirection::UR: x_moved++; y_moved--; break;
        case CanonDirection::UL: x_moved--; y_moved--; break;
        case CanonDirection::DR: x_moved++; y_moved++; break;
        case CanonDirection::DL: x_moved--; y_moved++; break;
    }

    if (x_moved < 0) x_moved += board_width;
    if (x_moved >= board_width) x_moved -= board_width;
    if (y_moved < 0) y_moved += board_height;
    if (y_moved >= board_height) y_moved -= board_height;
    
    return {x_moved, y_moved};
    // previousPosition = {x, y}; // Store previous position before moving
    // x = x_moved;
    // y = y_moved;
}

std::pair<int, int> Tank::moveBackward(int board_width, int board_height) {
    int x_moved = x, y_moved = y;

    switch (canon_dir) {
        case CanonDirection::U:  y_moved++; break;
        case CanonDirection::D:  y_moved--; break;
        case CanonDirection::L:  x_moved++; break;
        case CanonDirection::R:  x_moved--; break;
        case CanonDirection::UR: x_moved--; y_moved++; break;
        case CanonDirection::UL: x_moved++; y_moved++; break;
        case CanonDirection::DR: x_moved--; y_moved--; break;
        case CanonDirection::DL: x_moved++; y_moved--; break;
    }

    if (x_moved < 0) x_moved += board_width;
    if (x_moved >= board_width) x_moved -= board_width;
    if (y_moved < 0) y_moved += board_height;
    if (y_moved >= board_height) y_moved -= board_height;
    return {x_moved, y_moved};
    // previousPosition = {x, y}; // Store previous position before moving
    // x = x_moved;
    // y = y_moved;
}

// Rotations
void Tank::rotateEighthLeft() {
    switch (canon_dir) {
        case CanonDirection::U:  canon_dir = CanonDirection::UL; break;
        case CanonDirection::UR: canon_dir = CanonDirection::U; break;
        case CanonDirection::R:  canon_dir = CanonDirection::UR; break;
        case CanonDirection::DR: canon_dir = CanonDirection::R; break;
        case CanonDirection::D:  canon_dir = CanonDirection::DR; break;
        case CanonDirection::DL: canon_dir = CanonDirection::D; break;
        case CanonDirection::L:  canon_dir = CanonDirection::DL; break;
        case CanonDirection::UL: canon_dir = CanonDirection::L; break;
    }
}

void Tank::rotateEighthRight() {
    switch (canon_dir) {
        case CanonDirection::U:  canon_dir = CanonDirection::UR; break;
        case CanonDirection::UR: canon_dir = CanonDirection::R; break;
        case CanonDirection::R:  canon_dir = CanonDirection::DR; break;
        case CanonDirection::DR: canon_dir = CanonDirection::D; break;
        case CanonDirection::D:  canon_dir = CanonDirection::DL; break;
        case CanonDirection::DL: canon_dir = CanonDirection::L; break;
        case CanonDirection::L:  canon_dir = CanonDirection::UL; break;
        case CanonDirection::UL: canon_dir = CanonDirection::U; break;
    }
}

void Tank::rotateQuarterLeft() {
    switch (canon_dir) {
        case CanonDirection::U:  canon_dir = CanonDirection::L; break;
        case CanonDirection::UR: canon_dir = CanonDirection::UL; break;
        case CanonDirection::R:  canon_dir = CanonDirection::U; break;
        case CanonDirection::DR: canon_dir = CanonDirection::UR; break;
        case CanonDirection::D:  canon_dir = CanonDirection::R; break;
        case CanonDirection::DL: canon_dir = CanonDirection::DR; break;
        case CanonDirection::L:  canon_dir = CanonDirection::D; break;
        case CanonDirection::UL: canon_dir = CanonDirection::DL; break;
    }
}

void Tank::rotateQuarterRight() {
    switch (canon_dir) {
        case CanonDirection::U:  canon_dir = CanonDirection::R; break;
        case CanonDirection::UR: canon_dir = CanonDirection::DR; break;
        case CanonDirection::R:  canon_dir = CanonDirection::D; break;
        case CanonDirection::DR: canon_dir = CanonDirection::DL; break;
        case CanonDirection::D:  canon_dir = CanonDirection::L; break;
        case CanonDirection::DL: canon_dir = CanonDirection::UL; break;
        case CanonDirection::L:  canon_dir = CanonDirection::U; break;
        case CanonDirection::UL: canon_dir = CanonDirection::UR; break;
    }
}

// Actions Tracking
// void Tank::addAction(ActionRequest action) {
//     std::string action_str;
//     action_str = std::to_string(static_cast<int>(action));
//     my_actions_str.push_back(action_str);
//     my_actions.push_back(action);
// }

std::vector<std::string> Tank::getActions() const {
    return my_actions_str;
}

// Position Tracking
void Tank::setPreviousPosition() {
    previousPosition = {x, y};
}

std::pair<int, int> Tank::getPreviousPosition() const {
    return previousPosition;
}

// Destruction Tracking
void Tank::setDestructionCause(DestructionCause cause) {
    causeOfDestruction = cause;
}

DestructionCause Tank::getDestructionCause() const {
    return causeOfDestruction;
}

std::string Tank::getDestructionCauseStr() const {
    switch (causeOfDestruction) {
        case DestructionCause::NONE:      return "None";
        case DestructionCause::SHELL:     return "Destroyed by Shell";
        case DestructionCause::MINE:      return "Destroyed by Mine";
        case DestructionCause::TANK:      return "Destroyed by Tank Collision";
        case DestructionCause::MULTIPLE:  return "Destroyed by Multiple Factors";
        default:                          return "Unknown";
    }
}

// Basic Accessors
int Tank::getX() const { return x; }
int Tank::getY() const { return y; }
CanonDirection Tank::getCanonDirection() const { return canon_dir; }
char Tank::getIndexTank() const { return index_tank; }
int Tank::getNumBullets() const { return num_bullets; }
