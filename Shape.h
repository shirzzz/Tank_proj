//
// Created by Owner on 04/04/2025.
//

#ifndef SHAPE_H
#define SHAPE_H
#include "Direction.h"
//רציתי שזה יהיה מחלקה אבסטרקטית אבל לא הצלחתי

class Shape {
protected:
    int x, y;
    Direction direction;
public:
    Shape(int x, int y,Direction direction);
    void move();
    void update_position(int new_x, int new_y);
    int getX() const { return x; }
    int getY() const { return y; }
    Direction getDirection() const { return direction; }
    void setDirection(Direction new_direction) { direction = new_direction; }
    void setX(int new_x) { x = new_x; }
    void setY(int new_y) { y = new_y; }
};



#endif //SHAPE_H
