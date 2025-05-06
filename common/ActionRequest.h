
#ifndef ACTIONREQUEST_H
#define ACTIONREQUEST_H
#include <string>
#include <ostream>


enum class ActionReqeuest {
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

inline std::string toString(ActionReqeuest action) {
    switch (action) {
        case ActionReqeuest::MoveForward: return "MoveForward";
        case ActionReqeuest::MoveBackward: return "MoveBackward";
        case ActionReqeuest::RotateLeft90: return "RotateLeft90";
        case ActionReqeuest::RotateRight90: return "RotateRight90";
        case ActionReqeuest::RotateLeft45: return "RotateLeft45";
        case ActionReqeuest::RotateRight45: return "RotateRight45";
        case ActionReqeuest::Shoot: return "Shoot";
        case ActionReqeuest::GetBattleInfo: return "GetBattleInfo";
        case ActionReqeuest::DoNothing: return "DoNothing"; 
        default: return "BadStep";
    }
}

inline std::ostream& operator<<(std::ostream& os, ActionType action) {
    return os << toString(action);
};

#endif //ACTIONREQUEST_H
