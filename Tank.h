
#ifndef TANK_H
#define TANK_H
#include "CanonDirection.h"
#include "shape.h"
class Tank : public Shape {
    char index_tank;
    CanonDirection canon_dir;
    int num_bullets;
    int last_time_shoot;
    int last_time_backward;
public:
    Tank(int x, int y, Direction direction, char index_tank, CanonDirection canon_direction);
    void shoot();
    void rotate_eighth_left();
    void rotate_eighth_right();
    void rotate_quarter_left();
    void rotate_quarter_right();
    void move_forward();
    void move_backward();
};



#endif //TANK_H
