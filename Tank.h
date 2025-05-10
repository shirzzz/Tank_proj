#ifndef TANK_H
#define TANK_H

#include "CanonDirection.h"
#include "Shape.h"
#include "ActionType.h"
#include "DestructionCause.h"

#include <vector>
#include <string>
#include <utility>

class Tank : public Shape {
private:
    char index_tank;
    std::vector<ActionType> my_actions;
    CanonDirection canon_dir;
    int num_bullets;
    int last_time_shoot;
    int last_time_backward;

    int waiting_to_go_back = -1;       // Indicates pending backward move
    int waitng_to_shoot_again = -1;    // Indicates shooting cooldown

    std::string reason = "none";       // Human-readable reason for final state

    std::pair<int, int> previousPosition;               // For tracking last position
    DestructionCause causeOfDestruction = DestructionCause::NONE; // Cause of tank destruction

public:
    // Constructor
    Tank(int x, int y, char index_tank);

    // Actions
    void shoot();
    void rotateEighthLeft();
    void rotateEighthRight();
    void rotateQuarterLeft();
    void rotateQuarterRight();
    std::pair<int, int> moveForward(int board_width, int board_height);
    std::pair<int,int> moveBackward(int board_width, int board_height);

    // Accessors
    int getX() const;
    int getY() const;
    char getIndexTank() const;
    CanonDirection getCanonDirection() const;
    int getNumBullets() const;

    // Movement Logic
    void addAction(ActionType action);
    std::vector<ActionType> getActions() const;

    // Wait State Control
    int getWaitingToGoBack() const { return waiting_to_go_back; }
    void setWaitingToGoBack(int waiting) { waiting_to_go_back = waiting; }

    int getWaitingToShootAgain() const { return waitng_to_shoot_again; }
    void setWaitingToShootAgain(int waiting) { waitng_to_shoot_again = waiting; }

    // Final State Description
    void setReason(const std::string& reason) { this->reason = reason; }
    std::string getReason() const { return reason; }

    // Collision Tracking
    void setPreviousPosition();
    std::pair<int, int> getPreviousPosition() const;

    // Destruction Reporting
    void setDestructionCause(DestructionCause cause);
    DestructionCause getDestructionCause() const;
    std::string getDestructionCauseStr() const;
    void deleteFirstAction() { my_actions.erase(my_actions.begin()); } // Remove the first action
};

#endif // TANK_H
