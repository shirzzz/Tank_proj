
#ifndef ACTIONREQUEST_H
#define ACTIONREQUEST_H
#include <string>
#include <ostream>


enum class ActionRequest {
    MoveForward,
    MoveBackward,
    RotateLeft90,
    RotateRight90,
    RotateLeft45,
    RotateRight45,
    Shoot,
    GetBattleInfo,
    DoNothing
};

inline std::string toString(ActionRequest action) {
    switch (action) {
        case ActionRequest::MoveForward: return "MoveForward";
        case ActionRequest::MoveBackward: return "MoveBackward";
        case ActionRequest::RotateLeft90: return "RotateLeft90";
        case ActionRequest::RotateRight90: return "RotateRight90";
        case ActionRequest::RotateLeft45: return "RotateLeft45";
        case ActionRequest::RotateRight45: return "RotateRight45";
        case ActionRequest::Shoot: return "Shoot";
        case ActionRequest::GetBattleInfo: return "GetBattleInfo";
        case ActionRequest::DoNothing: return "DoNothing"; 
        default: return "BadStep";
    }
}

inline std::ostream& operator<<(std::ostream& os, ActionRequest action) {
    return os << toString(action);
};

#endif //ACTIONREQUEST_H
