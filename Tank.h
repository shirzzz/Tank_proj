
#ifndef TANK_H
#define TANK_H
#include "CanonDirection.h"
#include "shape.h"
#include "Direction.h"
class Tank : public Shape {
    char index_tank;
    CanonDirection canon_dir;
    int num_bullets;
    int last_time_shoot;
    int last_time_backward;
    Direction direction;
public:
    Tank(int x, int y,  CanonDirection canon_direction, char index_tank);
    void shoot();
    void rotate_eighth_left();
    void rotate_eighth_right();
    void rotate_quarter_left();
    void rotate_quarter_right();
    void move_forward();
    void move_backward();
    int getX() const;
    int getY() const;
    char getIndexTank() const;
    CanonDirection getCanonDirection() const;
    int getNumBullets() const;
    Direction getDirection() const;
    void moving_algorithm();
};



#endif //TANK_H
