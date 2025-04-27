
#ifndef ACTIONTYPE_H
#define ACTIONTYPE_H
#include <string>
#include <ostream>


enum class ActionType {
    MOVE_FORWARD,
    MOVE_BACKWARD,
    ROTATE_EIGHTH_LEFT,
    ROTATE_EIGHTH_RIGHT,
    ROTATE_QUARTER_LEFT,
    ROTATE_QUARTER_RIGHT,
    SHOOT,
    INVALID_ACTION,
    WIN,
    LOSE,
    DRAW,
};

inline std::string toString(ActionType action) {
    switch (action) {
        case ActionType::MOVE_FORWARD: return "MOVE_FORWARD";
        case ActionType::MOVE_BACKWARD: return "MOVE_BACKWARD";
        case ActionType::ROTATE_EIGHTH_LEFT: return "ROTATE_EIGHTH_LEFT";
        case ActionType::ROTATE_EIGHTH_RIGHT: return "ROTATE_EIGHTH_RIGHT";
        case ActionType::ROTATE_QUARTER_LEFT: return "ROTATE_QUARTER_LEFT";
        case ActionType::ROTATE_QUARTER_RIGHT: return "ROTATE_QUARTER_RIGHT";
        case ActionType::SHOOT: return "SHOOT";
        case ActionType::INVALID_ACTION: return "BAD_STEP";
        case ActionType::WIN: return "WIN";
        case ActionType::LOSE: return "LOSE";
        case ActionType::DRAW: return "DRAW";
        default: return "BAD_STEP";
    }
}

inline std::ostream& operator<<(std::ostream& os, ActionType action) {
    return os << toString(action);
};



#endif //ACTIONTYPE_H
