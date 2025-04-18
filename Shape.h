//
// Created by Owner on 04/04/2025.
//

#ifndef SHAPE_H
#define SHAPE_H
#include "CanonDirection.h"
#include <utility>
//רציתי שזה יהיה מחלקה אבסטרקטית אבל לא הצלחתי

class Shape {
protected:
    int x, y;
    CanonDirection direction;
public:

    virtual ~Shape() = default; // Virtual destructor for proper cleanup
    Shape(int x, int y,CanonDirection direction);
    Shape(int x, int y);
    void move();
    void updatePosition(int new_x, int new_y);
    int getX() const { return x; }
    int getY() const { return y; }
    CanonDirection getDirection() const { return direction; }
    void setDirection(CanonDirection new_direction) { direction = new_direction; }
    void setX(int new_x) { x = new_x; }
    void setY(int new_y) { y = new_y; }
    std::pair<int,int> getPosition() const { return {x, y}; }

};



#endif //SHAPE_H
