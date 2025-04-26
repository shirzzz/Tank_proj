#ifndef DESTRUCTIONCAUSE_H
#define DESTRUCTIONCAUSE_H


enum class DestructionCause {
    NONE,
    SHELL,
    MINE,
    TANK,
    MULTIPLE
};

inline std::string toString(DestructionCause cause) {
    switch (cause) {
        case DestructionCause::NONE: return "NONE";
        case DestructionCause::SHELL: return "SHELL HIT ME";
        case DestructionCause::MINE: return "I GOT ONTO A MINE";
        case DestructionCause::TANK: return "TANK COLLISION";
        case DestructionCause::MULTIPLE: return "MULTIPLE";
        default: return "UNKNOWN";
    }
}
inline std::ostream& operator<<(std::ostream& os, DestructionCause cause) {
    return os << toString(cause);
}

#endif