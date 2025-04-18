
#include "Tank.h"

#include <CanonDirection.h>
#include <BFSChaserAI.h>
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
void Tank::rotate_eighth_left() {
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
void Tank::rotate_eighth_right() {
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
void Tank::rotate_quarter_left() {
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
void Tank::rotate_quarter_right() {
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
void Tank::move_forward() {
    switch(canon_dir) {
        case CanonDirection::U:
            y++;
        break;
        case CanonDirection::D:
            y--;
        break;
        case CanonDirection::L:
            x--;
        break;
        case CanonDirection::R:
            x++;
        break;
        case CanonDirection::UR:
            x++;
        y++;
        break;
        case CanonDirection::UL:
            x--;
        y++;
        break;
        case CanonDirection::DR:
            x++;
        y--;
        break;
        case CanonDirection::DL:
            x--;
        y--;
        break;
    }
}
void Tank::move_backward() {
    //אני לא יודעת איך לדעת לאן אני לא יודעת איך לדעת לאן הוא זז
    switch(canon_dir) {
        case CanonDirection::U:
            y--;
        break;
        case CanonDirection::D:
            y++;
        break;
        case CanonDirection::L:
            x++;
        break;
        case CanonDirection::R:
            x--;
        break;
        case CanonDirection::UR:
            x--;
            y--;
        break;
        case CanonDirection::UL:
            x++;
            y--;
        break;
        case CanonDirection::DR:
            x--;
            y++;
        break;
        case CanonDirection::DL:
            x++;
            y++;
        break;
    }
}

int Tank::getX() const {
    return x;
}

int Tank::getY() const {
    return y;
}
CanonDirection Tank::getCanonDirection() const{
    return canon_dir;
}

char Tank::getIndexTank() const {
    return index_tank;
}


