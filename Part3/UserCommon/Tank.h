#ifndef TANK_H
#define TANK_H

#include "CanonDirection.h"
#include "Shape.h"
#include "../common/ActionRequest.h"
#include "../DestructionCause.h"

#include <vector>
#include <string>
#include <utility>

class Tank : public Shape {
private:
    char index_tank;
    std::pair<int, int> next_position;
    bool is_alive = true; // Track if the tank is alive
    std::vector<ActionRequest> my_actions;
    std::vector<std::string> my_actions_str; // For human-readable actions
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
    void addAction(std::string action) {
        my_actions_str.push_back(action);
    }
    const std::vector<std::string>& getActions() const;

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
    bool isAlive() const { return is_alive; }
    void killTank() {
         is_alive = false; 
         std::string last_action = my_actions_str.pop_back();
         my_actions_str.push_back(last_action + "(killed)");
    }
    void setCanonDirection(CanonDirection new_dir) { canon_dir = new_dir; }
    void setNextPosition(int x, int y, CanonDirection direction, int moving, int width, int height); //0 meanning no moving, 1 meaning moving forward, -1 meaning moving backward
    std::pair<int, int> getNextPosition() const { return next_position; }    
};

#endif // TANK_H
