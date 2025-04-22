
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
    int waiting_to_go_back = -1; //initialize value to -1, so we know that we are not in the middle of a backward move;
    int waitng_to_shoot_again = -1; //initialize value to -1, so we know that we are not in the middle of a shoot move;
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
    std::vector<ActionType> getActions() const;
    int getWatingToGoBack() const { return waiting_to_go_back; }
    void setWaitingToGoBack(int waiting) { waiting_to_go_back = waiting; }
    int getWaitingToShootAgain() const { return waitng_to_shoot_again; }
    void setWaitingToShootAgain(int waiting) { waitng_to_shoot_again = waiting; }
};



#endif //TANK_H
