
#include "Tank.h"

#include <CanonDirection.h>
#include <BFSChaserAI.h>
#include <Chased.h>
#include "shape.h"
#include <iostream>
Tank::Tank(int x, int y, CanonDirection canon_direction, char index_tank) : Shape(x, y, canon_direction), index_tank(index_tank), num_bullets(16), last_time_shoot(0), last_time_backward(0) {
    if(index_tank == '1') {
        this->Shape::direction = CanonDirection::L; // Tank 1 starts facing left
    }
    else {
        this->Shape::direction = CanonDirection::R; // Tank 2 starts facing right
    }
}
void Tank::shoot() {
    if (num_bullets > 0) {
        num_bullets--;
        // Logic to shoot a bullet from the tank's current position and direction
    }
    else {
        std::cout << "No bullets left!" << std::endl;
    }
}
void Tank::rotateEighthLeft() {
    // Rotate the tank's canon direction 45 degrees to the left
    switch (canon_dir) {
        case CanonDirection::U:
            canon_dir = CanonDirection::UL;
            break;
        case CanonDirection::UR:
            canon_dir = CanonDirection::U;
            break;
        case CanonDirection::R:
            canon_dir = CanonDirection::UR;
            break;
        case CanonDirection::DR:
            canon_dir = CanonDirection::R;
            break;
        case CanonDirection::D:
            canon_dir = CanonDirection::DR;
            break;
        case CanonDirection::DL:
            canon_dir = CanonDirection::D;
            break;
        case CanonDirection::L:
            canon_dir = CanonDirection::DL;
            break;
        case CanonDirection::UL:
            canon_dir = CanonDirection::L;
            break;
    }

}
void Tank::rotateEighthRight() {
    // Rotate the tank's canon direction 45 degrees to the right
    switch (canon_dir) {
        case CanonDirection::U:
            canon_dir = CanonDirection::UR;
            break;
        case CanonDirection::UR:
            canon_dir = CanonDirection::R;
            break;
        case CanonDirection::R:
            canon_dir = CanonDirection::DR;
            break;
        case CanonDirection::DR:
            canon_dir = CanonDirection::D;
            break;
        case CanonDirection::D:
            canon_dir = CanonDirection::DL;
            break;
        case CanonDirection::DL:
            canon_dir = CanonDirection::L;
            break;
        case CanonDirection::L:
            canon_dir = CanonDirection::UL;
            break;
        case CanonDirection::UL:
            canon_dir = CanonDirection::U;
            break;
    }
}
void Tank::rotateQuarterLeft() {
    // Rotate the tank's canon direction 90 degrees to the left
    switch (canon_dir) {
        case CanonDirection::U:
            canon_dir = CanonDirection::L;
            break;
        case CanonDirection::UR:
            canon_dir = CanonDirection::UL;
            break;
        case CanonDirection::R:
            canon_dir = CanonDirection::U;
            break;
        case CanonDirection::DR:
            canon_dir = CanonDirection::UR;
            break;
        case CanonDirection::D:
            canon_dir = CanonDirection::R;
            break;
        case CanonDirection::DL:
            canon_dir = CanonDirection::DR;
            break;
        case CanonDirection::L:
            canon_dir = CanonDirection::D;
            break;
        case CanonDirection::UL:
            canon_dir = CanonDirection::DL;
            break;
    }
}
void Tank::rotateQuarterRight() {
    // Rotate the tank's canon direction 90 degrees to the right
    switch (canon_dir) {
        case CanonDirection::U:
            canon_dir = CanonDirection::R;
            break;
        case CanonDirection::UR:
            canon_dir = CanonDirection::DR;
            break;
        case CanonDirection::R:
            canon_dir = CanonDirection::D;
            break;
        case CanonDirection::DR:
            canon_dir = CanonDirection::DL;
            break;
        case CanonDirection::D:
            canon_dir = CanonDirection::L;
            break;
        case CanonDirection::DL:
            canon_dir = CanonDirection::UL;
            break;
        case CanonDirection::L:
            canon_dir = CanonDirection::U;
            break;
        case CanonDirection::UL:
            canon_dir = CanonDirection::UR;
            break;
    }
}
void Tank::moveForward(int board_width, int board_height) {
    int x_moved = x;
    int y_moved = y;

    switch (canon_dir) {
        case CanonDirection::U:  y_moved++; break;
        case CanonDirection::D:  y_moved--; break;
        case CanonDirection::L:  x_moved--; break;
        case CanonDirection::R:  x_moved++; break;
        case CanonDirection::UR: x_moved++; y_moved++; break;
        case CanonDirection::UL: x_moved--; y_moved++; break;
        case CanonDirection::DR: x_moved++; y_moved--; break;
        case CanonDirection::DL: x_moved--; y_moved--; break;
    }

    // Wrap around horizontally
    if (x_moved < 0) x_moved += board_width;
    if (x_moved >= board_width) x_moved -= board_width;

    // Wrap around vertically
    if (y_moved < 0) y_moved += board_height;
    if (y_moved >= board_height) y_moved -= board_height;

    // Apply the move
    x = x_moved;
    y = y_moved;
}

void Tank::moveBackward(int board_width, int board_height) {
    int x_moved = x;
    int y_moved = y;
    switch (canon_dir) {
        case CanonDirection::U: y_moved--;
            break;
        case CanonDirection::D: y_moved++;
            break;
        case CanonDirection::L: x_moved++;
            break;
        case CanonDirection::R: x_moved--;
            break;
        case CanonDirection::UR: x_moved--;
            y_moved--;
            break;
        case CanonDirection::UL: x_moved++;
            y_moved--;
            break;
        case CanonDirection::DR: x_moved--;
            y_moved++;
            break;
        case CanonDirection::DL: x_moved++;
            y_moved++;
            break;
    }

    // Wrap around horizontally
    if (x_moved < 0) x_moved += board_width;
    if (x_moved >= board_width) x_moved -= board_width;

    // Wrap around vertically
    if (y_moved < 0) y_moved += board_height;
    if (y_moved >= board_height) y_moved -= board_height;

    // Apply the move
    x = x_moved;
    y = y_moved;
}


int Tank::getX() const {
    return x;
}

int Tank::getY() const {
    return y;
}

CanonDirection Tank::getCanonDirection() const {
    return canon_dir;
}

char Tank::getIndexTank() const {
    return index_tank;
}

int Tank::getNumBullets() const {
    return num_bullets;
}
ActionType Tank::movingAlgorithm(GameBoard & game_board) {
    if(index_tank == '1') {
        // Tank 1 (AI Chaser)
        BFSChaserAI ai1;
        ActionType action1 = ai1.decideNextAction(game_board, *game_board.getTank1(), *game_board.getTank2());
        return action1;
    }
        // Tank 2 (Chased)
        Chased chased;
        ActionType action2 = chased.decideNextAction(game_board, *game_board.getTank2(), *game_board.getTank1());
        return action2;
}
void Tank::addAction(ActionType action) {
    my_actions.push_back(action);
}
std::vector<ActionType> Tank::getActions() const {
    return my_actions;
}

