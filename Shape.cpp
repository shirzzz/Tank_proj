//
// Created by Owner on 04/04/2025.
//

#include "shape.h"
Shape::Shape(int x, int y, CanonDirection direction) : x(x), y(y), direction(direction) {
    // Initialize the shape's position and dimensions
}
void Shape::move() {
    // Move the shape in the current direction
    switch (direction) {
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
        case CanonDirection::UL:
            x--;
            y++;
        break;
        case CanonDirection::UR:
            x++;
            y++;
        break;
        case CanonDirection::DL:
            x--;
            y--;
        break;
        case CanonDirection::DR:
            x++;
            y--;
        break;
    }
}
void Shape::updatePosition(int new_x, int new_y) {
    x= new_x;
    y = new_y;
}
