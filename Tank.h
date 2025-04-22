
#ifndef TANK_H
#define TANK_H
#include "CanonDirection.h"
#include "shape.h"
#include <vector>
#include "ActionType.h"
#include "GameBoard.h"

class Tank : public Shape {
    char index_tank;
    std::vector<ActionType> my_actions;
    CanonDirection canon_dir;
    int num_bullets;
    int last_time_shoot;
    int last_time_backward;

public:
    Tank(int x, int y,  CanonDirection canon_direction, char index_tank);
    void shoot();
    void rotateEighthLeft();
    void rotateEighthRight();
    void rotateQuarterLeft();
    void rotateQuarterRight();
    void moveForward();
    void moveBackward();
    int getX() const;
    int getY() const;
    char getIndexTank() const;
    CanonDirection getCanonDirection() const;
    int getNumBullets() const;
    ActionType movingAlgorithm(GameBoard & game_board);
    void addAction(ActionType action);
};



#endif //TANK_H
