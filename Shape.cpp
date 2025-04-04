//
// Created by Owner on 04/04/2025.
//

#include "shape.h"
Shape::Shape(int x, int y, Direction direction) : x(x), y(y), direction(direction) {
    // Initialize the shape's position and dimensions
}
void Shape::move() {
    // Move the shape in the current direction
    switch (direction) {
        case Direction::UP:
            y++;
        break;
        case Direction::DOWN:
            y--;
        break;
        case Direction::LEFT:
            x--;
        break;
        case Direction::RIGHT:
            x++;
        break;
        case Direction::UP_LEFT:
            x--;
        y++;
        break;
        case Direction::UP_RIGHT:
            x++;
        y++;
        break;
        case Direction::DOWN_LEFT:
            x--;
        y--;
        break;
        case Direction::DOWN_RIGHT:
            x++;
        y--;
        break;
    }
}
void Shape::update_position(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}